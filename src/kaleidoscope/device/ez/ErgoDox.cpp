/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-EZ-ErgoDox -- ErgoDox hardware support for Kaleidoscope
 * Copyright (C) 2018, 2019  Keyboard.io, Inc
 *
 * Based on QMK (commit 8cdb4a915)
 *  (C) Jack Humbert, Erez Zukerman, Oleg Kostyuk
 * Original QMK sources:
 *  - keyboards/ergodox_ez/ergodox_ez.c
 *  - keyboards/ergodox_ez/ergodox_ez.h
 *  - keyboards/ergodox_ez/matrix.c
 *  - tmk_core/common/avr/bootloader.c
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
#ifdef ARDUINO_AVR_ERGODOX

#include "kaleidoscope/Runtime.h"
#include <avr/wdt.h>
#include "kaleidoscope/device/ez/ErgoDox/ErgoDoxScanner.h"
#include "kaleidoscope/key_events.h"

namespace kaleidoscope {
namespace device {
namespace ez {

ErgoDoxScanner ErgoDox::scanner_;
ErgoDox::row_state_t ErgoDox::matrix_state_[ErgoDoxProps::KeyScannerProps::matrix_rows];

static bool do_scan_ = 1;

void ErgoDox::setup(void) {
  wdt_disable();
  delay(100);

  TCCR1A = 0b10101001;
  TCCR1B = 0b00001001;

  DDRB  &= ~(1 << 4);
  PORTB &= ~(1 << 4);

  DDRC  &= ~(1 << 7);
  DDRD  &= ~(1 << 5 | 1 << 4);
  DDRE  &= ~(1 << 6);
  PORTC |= (1 << 7);
  PORTD |= (1 << 5 | 1 << 4);
  PORTE |= (1 << 6);

  scanner_.begin();

  setStatusLEDBrightness(1, 15);
  setStatusLEDBrightness(2, 15);
  setStatusLEDBrightness(3, 15);

  /* Set up Timer1 for 1700usec */
  TCCR1B = _BV(WGM13);
  TCCR1A = 0;

  const uint32_t cycles = (F_CPU / 2000000) * 1700;

  ICR1 = cycles;
  TCCR1B = _BV(WGM13) | _BV(CS10);
  TIMSK1 = _BV(TOIE1);

  hid_.setup();
}

ISR(TIMER1_OVF_vect) {
  do_scan_ = true;
}

void __attribute__((optimize(3))) ErgoDox::readMatrix() {
  uint8_t any_debounced_changes = 0;

  do_scan_ = false;

  scanner_.reattachExpanderOnError();

  for (uint8_t row = 0; row < matrix_rows / 2; row++) {
    scanner_.selectExtenderRow(row);
    scanner_.toggleATMegaRow(row);

    uint8_t hot_pins = scanner_.readCols(row);
    any_debounced_changes |= debouncer_.debounce(hot_pins, row);

    uint8_t hot_extender_pins = scanner_.readCols(row + matrix_rows / 2);
    any_debounced_changes |= debouncer_.debounce(hot_extender_pins, row + matrix_rows / 2);

    scanner_.toggleATMegaRow(row);
  }

  if (any_debounced_changes) {
    for (uint8_t row = 0; row < matrix_rows; row++) {
      matrix_state_[row].current = debouncer_.getRowState(row);
    }
  }
}

void __attribute__((optimize(3))) ErgoDox::actOnMatrixScan() {
  for (byte row = 0; row < matrix_rows; row++) {
    for (byte col = 0; col < matrix_columns; col++) {
      uint8_t keyState = (bitRead(matrix_state_[row].previous, col) << 0) |
                         (bitRead(matrix_state_[row].current, col) << 1);
      if (keyState)
        handleKeyswitchEvent(Key_NoKey, KeyAddr(row, col), keyState);
    }
    matrix_state_[row].previous = matrix_state_[row].current;
  }
}

void ErgoDox::scanMatrix() {
  if (do_scan_) {
    do_scan_ = false;
    // We only want to update our matrix if the timer has expired.
    readMatrix();
  }

  // We ALWAYS want to tell Kaleidoscope about the state of the matrix
  actOnMatrixScan();
}

void ErgoDox::maskKey(KeyAddr key_addr) {
  if (!key_addr.isValid())
    return;

  bitWrite(matrix_state_[key_addr.row()].masks, key_addr.col(), 1);
}

void ErgoDox::unMaskKey(KeyAddr key_addr) {
  if (!key_addr.isValid())
    return;

  bitWrite(matrix_state_[key_addr.row()].masks, key_addr.col(), 0);
}

bool ErgoDox::isKeyMasked(KeyAddr key_addr) {
  if (!key_addr.isValid())
    return false;

  return bitRead(matrix_state_[key_addr.row()].masks, key_addr.col());
}

// ErgoDox-specific stuff
void ErgoDox::setStatusLED(uint8_t led, bool state) {
  if (state) {
    DDRB |= (1 << (led + 4));
    PORTB |= (1 << (led + 4));
  } else {
    DDRB &= ~(1 << (led + 4));
    PORTB &= ~(1 << (led + 4));
  }
}

void ErgoDox::setStatusLEDBrightness(uint8_t led, uint8_t brightness) {
  (led == 1) ? (OCR1A = brightness) :
  (led == 2) ? (OCR1B = brightness) :
  (OCR1C = brightness);
}

bool ErgoDox::isKeyswitchPressed(KeyAddr key_addr) {
  return (bitRead(matrix_state_[key_addr.row()].current, key_addr.col()) != 0);
}

bool ErgoDox::wasKeyswitchPressed(KeyAddr key_addr) {
  return (bitRead(matrix_state_[key_addr.row()].previous, key_addr.col()) != 0);
}

uint8_t ErgoDox::previousPressedKeyswitchCount() {
  uint8_t count = 0;

  for (uint8_t r = 0; r < matrix_rows; r++) {
    count += __builtin_popcount(matrix_state_[r].previous);
  }
  return count;
}

uint8_t ErgoDox::pressedKeyswitchCount() {
  uint8_t count = 0;

  for (uint8_t r = 0; r < matrix_rows; r++) {
    count += __builtin_popcount(matrix_state_[r].current);
  }
  return count;
}

}
}
}

#endif
#endif // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

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

#include <avr/wdt.h>

#include "kaleidoscope/KeyEvent.h"
#include "kaleidoscope/Runtime.h"
#include "kaleidoscope/device/ez/ErgoDox/ErgoDoxScanner.h"
#include "kaleidoscope/keyswitch_state.h"

namespace kaleidoscope {
namespace device {
namespace ez {

static bool do_scan_ = 1;

void ErgoDox::setup() {
  wdt_disable();
  delay(100);

  TCCR1A = 0b10101001;
  TCCR1B = 0b00001001;

  DDRB &= ~(1 << 4);
  PORTB &= ~(1 << 4);

  DDRC &= ~(1 << 7);
  DDRD &= ~(1 << 5 | 1 << 4);
  DDRE &= ~(1 << 6);
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

  ICR1   = cycles;
  TCCR1B = _BV(WGM13) | _BV(CS10);
  TIMSK1 = _BV(TOIE1);

  hid_.setup();
}

ISR(TIMER1_OVF_vect) {
  do_scan_ = true;
}

void __attribute__((optimize(3))) ErgoDox::readMatrixRow(uint8_t row) {
  uint8_t mask, cols;

  mask = debounceMaskForRow(row);
  cols = (scanner_.readCols(row) & mask) | (keyState_[row] & ~mask);
  debounceRow(cols ^ keyState_[row], row);
  keyState_[row] = cols;
}

void __attribute__((optimize(3))) ErgoDox::readMatrix() {
  do_scan_ = false;

  scanner_.reattachExpanderOnError();

  for (uint8_t row = 0; row < matrix_rows / 2; row++) {
    scanner_.selectExtenderRow(row);
    scanner_.toggleATMegaRow(row);

    readMatrixRow(row);
    readMatrixRow(row + matrix_rows / 2);

    scanner_.toggleATMegaRow(row);
  }
}

void __attribute__((optimize(3))) ErgoDox::actOnMatrixScan() {
  for (uint8_t row = 0; row < matrix_rows; row++) {
    for (uint8_t col = 0; col < matrix_columns; col++) {
      uint8_t key_state = (bitRead(previousKeyState_[row], col) << 0) |
                          (bitRead(keyState_[row], col) << 1);
      if (keyToggledOn(key_state) || keyToggledOff(key_state)) {
        auto event = KeyEvent::next(KeyAddr(row, col), key_state);
        kaleidoscope::Runtime.handleKeyswitchEvent(event);
      }
    }
    previousKeyState_[row] = keyState_[row];
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
  (led == 1) ? (OCR1A = brightness) : (led == 2) ? (OCR1B = brightness)
                                                 : (OCR1C = brightness);
}

uint8_t ErgoDox::debounceMaskForRow(uint8_t row) {
  uint8_t result = 0;

  for (uint8_t c = 0; c < matrix_columns; ++c) {
    if (debounce_matrix_[row][c]) {
      --debounce_matrix_[row][c];
    } else {
      result |= (1 << c);
    }
  }
  return result;
}

void ErgoDox::debounceRow(uint8_t change, uint8_t row) {
  for (uint8_t i = 0; i < matrix_columns; ++i) {
    if (change & (1 << i)) {
      debounce_matrix_[row][i] = debounce;
    }
  }
}

bool ErgoDox::isKeyswitchPressed(KeyAddr key_addr) {
  return (bitRead(keyState_[key_addr.row()], key_addr.col()) != 0);
}

bool ErgoDox::isKeyswitchPressed(uint8_t keyIndex) {
  keyIndex--;
  return isKeyswitchPressed(KeyAddr(keyIndex));
}

bool ErgoDox::wasKeyswitchPressed(KeyAddr key_addr) {
  return (bitRead(previousKeyState_[key_addr.row()], key_addr.col()) != 0);
}

bool ErgoDox::wasKeyswitchPressed(uint8_t keyIndex) {
  keyIndex--;
  return wasKeyswitchPressed(KeyAddr(keyIndex));
}

uint8_t ErgoDox::previousPressedKeyswitchCount() {
  uint8_t count = 0;

  for (uint8_t r = 0; r < matrix_rows; r++) {
    count += __builtin_popcount(previousKeyState_[r]);
  }
  return count;
}

uint8_t ErgoDox::pressedKeyswitchCount() {
  uint8_t count = 0;

  for (uint8_t r = 0; r < matrix_rows; r++) {
    count += __builtin_popcount(keyState_[r]);
  }
  return count;
}

}  // namespace ez
}  // namespace device
}  // namespace kaleidoscope

#endif
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

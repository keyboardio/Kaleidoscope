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

#ifdef ARDUINO_AVR_ERGODOX

#include <Kaleidoscope.h>
#include <KeyboardioHID.h>
#include <avr/wdt.h>
#include "kaleidoscope/hardware/ez/ErgoDox/ErgoDoxScanner.h"

namespace kaleidoscope {
namespace hardware {
namespace ez {

ErgoDoxScanner ErgoDox::scanner_;
uint8_t ErgoDox::previousKeyState_[ROWS];
uint8_t ErgoDox::keyState_[ROWS];
uint8_t ErgoDox::masks_[ROWS];
uint8_t ErgoDox::debounce_matrix_[ROWS][COLS];
uint8_t ErgoDox::debounce = 5;

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

  for (uint8_t row = 0; row < ROWS / 2; row++) {
    scanner_.selectExtenderRow(row);
    scanner_.toggleATMegaRow(row);

    readMatrixRow(row);
    readMatrixRow(row + ROWS / 2);

    scanner_.toggleATMegaRow(row);
  }
}

void __attribute__((optimize(3))) ErgoDox::actOnMatrixScan() {
  for (byte row = 0; row < ROWS; row++) {
    for (byte col = 0; col < COLS; col++) {
      uint8_t keyState = (bitRead(previousKeyState_[row], col) << 0) |
                         (bitRead(keyState_[row], col) << 1);
      if (keyState)
        handleKeyswitchEvent(Key_NoKey, KeyAddr(row, col), keyState);
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

void ErgoDox::maskKey(KeyAddr key_addr) {
  if (!key_addr.isValid())
    return;

  bitWrite(masks_[key_addr.row()], key_addr.col(), 1);
}

void ErgoDox::unMaskKey(KeyAddr key_addr) {
  if (!key_addr.isValid())
    return;

  bitWrite(masks_[key_addr.row()], key_addr.col(), 0);
}

bool ErgoDox::isKeyMasked(KeyAddr key_addr) {
  if (!key_addr.isValid())
    return false;

  return bitRead(masks_[key_addr.row()], key_addr.col());
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

void ErgoDox::resetDevice() {
  cli();
  UDCON = 1;
  USBCON = (1 << FRZCLK);
  UCSR1B = 0;
  _delay_ms(5);

  EIMSK = 0;
  PCICR = 0;
  SPCR = 0;
  ACSR = 0;
  EECR = 0;
  ADCSRA = 0;
  TIMSK0 = 0;
  TIMSK1 = 0;
  TIMSK3 = 0;
  TIMSK4 = 0;
  UCSR1B = 0;
  TWCR = 0;
  DDRB = 0;
  DDRC = 0;
  DDRD = 0;
  DDRE = 0;
  DDRF = 0;
  TWCR = 0;
  PORTB = 0;
  PORTC = 0;
  PORTD = 0;
  PORTE = 0;
  PORTF = 0;
  asm volatile("jmp 0x7E00");
}


uint8_t ErgoDox::debounceMaskForRow(uint8_t row) {
  uint8_t result = 0;

  for (uint8_t c = 0; c < COLS; ++c) {
    if (debounce_matrix_[row][c]) {
      --debounce_matrix_[row][c];
    } else {
      result |= (1 << c);
    }
  }
  return result;
}

void ErgoDox::debounceRow(uint8_t change, uint8_t row) {
  for (uint8_t i = 0; i < COLS; ++i) {
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

  for (uint8_t r = 0; r < ROWS; r++) {
    count += __builtin_popcount(previousKeyState_[r]);
  }
  return count;
}

uint8_t ErgoDox::pressedKeyswitchCount() {
  uint8_t count = 0;

  for (uint8_t r = 0; r < ROWS; r++) {
    count += __builtin_popcount(keyState_[r]);
  }
  return count;
}

}
}
}

HARDWARE_IMPLEMENTATION KeyboardHardware;
kaleidoscope::hardware::ez::ErgoDox &ErgoDox = KeyboardHardware;

#endif

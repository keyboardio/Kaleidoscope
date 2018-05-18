/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-ErgoDox -- ErgoDox hardware support for Kaleidoscope
 * Copyright (C) 2018  Gergely Nagy
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

#include <Kaleidoscope.h>
#include <KeyboardioHID.h>
#include <avr/wdt.h>
#include "ErgoDoxScanner.h"

namespace kaleidoscope {
namespace hardware {

ErgoDoxScanner ErgoDox::scanner_;
uint8_t ErgoDox::previousKeyState_[ROWS];
uint8_t ErgoDox::keyState_[ROWS];
uint8_t ErgoDox::masks_[ROWS];

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
}

void ErgoDox::readMatrix() {
  for (uint8_t i = 0; i < ROWS / 2; i++) {
    scanner_.selectRow(i);
    scanner_.selectRow(i + ROWS / 2);

    // TODO(algernon): debouncing

    // left side
    previousKeyState_[i] = keyState_[i];
    keyState_[i] = scanner_.readCols(i);

    // right side
    previousKeyState_[i + ROWS / 2] = keyState_[i + ROWS / 2];
    keyState_[i + ROWS / 2] = scanner_.readCols(i + ROWS / 2);

    scanner_.unselectRows();
  }
}

void ErgoDox::actOnMatrixScan() {
  for (byte row = 0; row < ROWS; row++) {
    for (byte col = 0; col < COLS; col++) {
      uint8_t keyState = (bitRead(previousKeyState_[row], col) << 0) |
                         (bitRead(keyState_[row], col) << 1);
      handleKeyswitchEvent(Key_NoKey, row, col, keyState);
    }
  }
}

void ErgoDox::scanMatrix() {
  readMatrix();
  actOnMatrixScan();
}

void ErgoDox::maskKey(byte row, byte col) {
  if (row >= ROWS || col >= COLS)
    return;

  bitWrite(masks_[row], col, 1);
}

void ErgoDox::unMaskKey(byte row, byte col) {
  if (row >= ROWS || col >= COLS)
    return;

  bitWrite(masks_[row], col, 0);
}

bool ErgoDox::isKeyMasked(byte row, byte col) {
  if (row >= ROWS || col >= COLS)
    return false;

  return bitRead(masks_[row], col);
}

}
}

HARDWARE_IMPLEMENTATION KeyboardHardware;
kaleidoscope::hardware::ErgoDox &ErgoDox = KeyboardHardware;

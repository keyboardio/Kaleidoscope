/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-ATMegaKeyboard -- Base class for some ATMega-based boards
 * Copyright (C) 2018  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#if ARDUINO_AVR_PLANCK || ARDUINO_AVR_ATREUS

#include "Kaleidoscope.h"
#include "kaleidoscope/hardware/ATMegaKeyboard.h"

namespace kaleidoscope {
namespace hardware {

uint8_t ATMegaKeyboard::debounce = 3;

void ATMegaKeyboard::setup(void) {
  wdt_disable();

  for (uint8_t i = 0; i < KeyboardHardware.matrix_columns; i++) {
    DDR_INPUT(KeyboardHardware.matrix_col_pins[i]);
    ENABLE_PULLUP(KeyboardHardware.matrix_col_pins[i]);
  }

  for (uint8_t i = 0; i < KeyboardHardware.matrix_rows; i++) {
    DDR_OUTPUT(KeyboardHardware.matrix_row_pins[i]);
    OUTPUT_HIGH(KeyboardHardware.matrix_row_pins[i]);
  }

  /* Set up Timer1 for 500usec */
  TCCR1B = _BV(WGM13);
  TCCR1A = 0;

  const uint32_t cycles = (F_CPU / 2000000) * 500;

  ICR1 = cycles;
  TCCR1B = _BV(WGM13) | _BV(CS10);
  TIMSK1 = _BV(TOIE1);
}

void ATMegaKeyboard::detachFromHost() {
  UDCON |= _BV(DETACH);
}

void ATMegaKeyboard::attachToHost() {
  UDCON &= ~_BV(DETACH);
}

void ATMegaKeyboard::readMatrix(void) {
  for (uint8_t current_row = 0; current_row < KeyboardHardware.matrix_rows; current_row++) {
    uint16_t mask, cols;

    KeyboardHardware.previousKeyState_[current_row] = KeyboardHardware.keyState_[current_row];

    mask = debounceMaskForRow(current_row);

    OUTPUT_TOGGLE(KeyboardHardware.matrix_row_pins[current_row]);
    cols = (readCols() & mask) | (KeyboardHardware.keyState_[current_row] & ~mask);
    OUTPUT_TOGGLE(KeyboardHardware.matrix_row_pins[current_row]);
    debounceRow(cols ^ KeyboardHardware.keyState_[current_row], current_row);
    KeyboardHardware.keyState_[current_row] = cols;
  }
}

uint8_t ATMegaKeyboard::pressedKeyswitchCount() {
  uint8_t count = 0;

  for (int8_t r = 0; r < KeyboardHardware.matrix_rows; r++) {
    count += __builtin_popcount(KeyboardHardware.keyState_[r]);
  }
  return count;
}

bool ATMegaKeyboard::isKeyswitchPressed(uint8_t row, byte col) {
  return (bitRead(KeyboardHardware.keyState_[row], col) != 0);

}

bool ATMegaKeyboard::isKeyswitchPressed(uint8_t keyIndex) {
  keyIndex--;
  return isKeyswitchPressed(keyIndex / KeyboardHardware.matrix_columns,
                            keyIndex % KeyboardHardware.matrix_columns);
}

void ATMegaKeyboard::actOnMatrixScan() {
  for (byte row = 0; row < KeyboardHardware.matrix_rows; row++) {
    for (byte col = 0; col < KeyboardHardware.matrix_columns; col++) {
      uint8_t keyState = (bitRead(KeyboardHardware.previousKeyState_[row], col) << 0) |
                         (bitRead(KeyboardHardware.keyState_[row], col) << 1);
      if (keyState) {
        handleKeyswitchEvent(Key_NoKey, row, col, keyState);
      }
    }
    KeyboardHardware.previousKeyState_[row] = KeyboardHardware.keyState_[row];
  }
}

void ATMegaKeyboard::scanMatrix() {
  if (!do_scan_)
    return;

  do_scan_ = false;

  readMatrix();
  actOnMatrixScan();
}

void ATMegaKeyboard::maskKey(byte row, byte col) {
  if (row >= KeyboardHardware.matrix_rows || col >= KeyboardHardware.matrix_columns)
    return;

  bitWrite(KeyboardHardware.masks_[row], col, 1);
}

void ATMegaKeyboard::unMaskKey(byte row, byte col) {
  if (row >= KeyboardHardware.matrix_rows || col >= KeyboardHardware.matrix_columns)
    return;

  bitWrite(KeyboardHardware.masks_[row], col, 0);
}

bool ATMegaKeyboard::isKeyMasked(byte row, byte col) {
  if (row >= KeyboardHardware.matrix_rows || col >= KeyboardHardware.matrix_columns)
    return false;

  return bitRead(KeyboardHardware.masks_[row], col);
}

bool ATMegaKeyboard::do_scan_;

uint16_t ATMegaKeyboard::readCols() {
  uint16_t results = 0x00 ;
  for (uint8_t i = 0; i < KeyboardHardware.matrix_columns; i++) {
    results |= (!READ_PIN(KeyboardHardware.matrix_col_pins[i]) << i);
  }
  return results;
}

uint16_t ATMegaKeyboard::debounceMaskForRow(uint8_t row) {
  uint16_t result = 0;

  for (uint16_t c = 0; c < KeyboardHardware.matrix_columns; ++c) {
    if (KeyboardHardware.debounce_matrix_[row][c]) {
      --KeyboardHardware.debounce_matrix_[row][c];
    } else {
      result |= _BV(c);
    }
  }
  return result;
}

void ATMegaKeyboard::debounceRow(uint16_t change, uint8_t row) {
  for (uint16_t i = 0; i < KeyboardHardware.matrix_columns; ++i) {
    if (change & _BV(i)) {
      KeyboardHardware.debounce_matrix_[row][i] = debounce;
    }
  }
}

}
}

#endif

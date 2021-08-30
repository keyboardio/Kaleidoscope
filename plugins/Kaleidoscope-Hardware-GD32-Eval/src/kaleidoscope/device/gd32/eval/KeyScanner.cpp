/* -*- mode: c++ -*-
 * Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2021  Keyboard.io, Inc.
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

#ifdef ARDUINO_GD32F303ZE_EVAL

#include "Arduino.h"
#include "kaleidoscope/Runtime.h"
#include "kaleidoscope/device/gd32/eval/KeyScanner.h"

namespace kaleidoscope {
namespace device {
namespace gd32 {
namespace eval {

KeyScanner::col_state_t KeyScanner::matrix_state_[KeyScannerProps::matrix_columns];
const uint8_t KeyScannerProps::matrix_rows;
const uint8_t KeyScannerProps::matrix_columns;
constexpr uint8_t KeyScannerProps::matrix_row_pins[matrix_rows];
constexpr uint8_t KeyScannerProps::matrix_col_pins[matrix_columns];

void KeyScanner::setup() {
  for (uint8_t i = 0; i < Props_::matrix_columns; i++) {
    pinMode(Props_::matrix_row_pins[i], INPUT_PULLUP);
  }

  for (uint8_t i = 0; i < Props_::matrix_rows; i++) {
    pinMode(Props_::matrix_row_pins[i], INPUT);
  }
}

void KeyScanner::scanMatrix() {
  readMatrix();
  actOnMatrixScan();
}

uint16_t KeyScanner::readRows() {
  uint16_t hot_pins = 0;

  for (uint8_t i = 0; i < Props_::matrix_rows; i++) {
    uint8_t rowPin = Props_::matrix_row_pins[i];
    pinMode(rowPin, INPUT_PULLUP);
    uint8_t v = !!digitalRead(rowPin);
    pinMode(rowPin, INPUT);
    hot_pins |= v << i;
  }

  return hot_pins;
}

uint16_t KeyScanner::debounce(uint16_t sample, debounce_t *debouncer) {
  uint16_t delta, changes;

  // Use xor to detect changes from last stable state:
  // if a key has changed, it's bit will be 1, otherwise 0
  delta = sample ^ debouncer->debounced_state;

  // Increment counters and reset any unchanged bits:
  // increment bit 1 for all changed keys
  debouncer->db1 = ((debouncer->db1) ^ (debouncer->db0)) & delta;
  // increment bit 0 for all changed keys
  debouncer->db0 = ~(debouncer->db0) & delta;

  // Calculate returned change set: if delta is still true
  // and the counter has wrapped back to 0, the key is changed.

  changes = ~(~delta | (debouncer->db0) | (debouncer->db1));
  // Update state: in this case use xor to flip any bit that is true in changes.
  debouncer->debounced_state ^= changes;

  return changes;
}

void KeyScanner::readMatrix() {
  uint16_t any_debounced_changes = 0;

  for (uint8_t col = 0; col < Props_::matrix_columns; col++) {
    uint8_t colPin = Props_::matrix_col_pins[col];
    pinMode(colPin, OUTPUT);
    digitalWrite(colPin, LOW);

    uint16_t hot_pins = readRows();
    pinMode(colPin, INPUT);

    any_debounced_changes |= debounce(hot_pins, &matrix_state_[col].debouncer);

    if (any_debounced_changes) {
      for (uint8_t i = 0; i < Props_::matrix_columns; i++) {
        matrix_state_[i].current = matrix_state_[i].debouncer.debounced_state;
      }
    }
  }
}

void KeyScanner::actOnMatrixScan() {
  for (byte col = 0; col < Props_::matrix_columns; col++) {
    for (byte row = 0; row < Props_::matrix_rows; row++) {
      uint8_t keyState = (bitRead(matrix_state_[col].previous, row) << 0) | (bitRead(matrix_state_[col].current, row) << 1);
      if (keyState) {
        ThisType::handleKeyswitchEvent(Key_NoKey, typename Props_::KeyAddr(row, col), keyState);
      }
    }
    matrix_state_[col].previous = matrix_state_[col].current;
  }
}

} // namespace eval
} // namespace gd32
} // namespace device
} // namespace kaleidoscope

#endif

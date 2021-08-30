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

#pragma once

#ifdef ARDUINO_GD32F303ZE_EVAL

#include <Arduino.h>

#include "kaleidoscope/device/gd32/eval/KeyScanner.h"

namespace kaleidoscope {
namespace device {
namespace gd32 {
namespace eval {

struct KeyScannerProps : public kaleidoscope::driver::keyscanner::BaseProps {
  static constexpr uint8_t matrix_rows = 1;
  static constexpr uint8_t matrix_columns = 2;

  static constexpr uint8_t matrix_row_pins[] = { PA3  };
  static constexpr uint8_t matrix_col_pins[] = { PE4, PD12 };

  typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;
};

class KeyScanner: public kaleidoscope::driver::keyscanner::Base<KeyScannerProps> {
 private:
  typedef KeyScanner ThisType;
  typedef KeyScannerProps Props_;
 public:
  static bool do_scan;

  static void setup();
  static void scanMatrix();
  static void readMatrix();
  static void actOnMatrixScan();

  static bool isKeyswitchPressed(KeyAddr key_addr);
  static uint8_t pressedKeyswitchCount();

  static bool wasKeyswitchPressed(KeyAddr key_addr);
  static uint8_t previousPressedKeyswitchCount();
 private:
  /*
    each of these variables are storing the state for a row of keys

    so for key 0, the counter is represented by db0[0] and db1[0]
    and the state in debounced_state[0].
  */
  struct debounce_t {
    uint16_t db0;    // counter bit 0
    uint16_t db1;    // counter bit 1
    uint16_t debounced_state;  // debounced state
  };

  struct col_state_t {
    uint16_t previous;
    uint16_t current;
    debounce_t debouncer;
  };
  static col_state_t matrix_state_[Props_::matrix_columns];

  static inline uint16_t debounce(uint16_t sample, debounce_t *debouncer);
  static uint16_t readRows();
};

} // namespace eval
} // namespace gd32
} // namespace device
} // namespace kaleidoscope

#endif

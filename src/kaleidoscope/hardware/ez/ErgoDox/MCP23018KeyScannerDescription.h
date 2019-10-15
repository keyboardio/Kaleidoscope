/* -*- mode: c++ -*-
 * MCP23018KeyScannerDescription -- MCP23018-based keyscanner component, description
 * Copyright (C) 2018-2019  Keyboard.io, Inc
 *
 * Based on QMK (commit 8cdb4a915)
 *  (C) Jack Humbert, Erez Zukerman, Oleg Kostyuk
 * Original QMK sources:
 *  - keyboards/ergodox_ez/ergodox_ez.c
 *  - keyboards/ergodox_ez/ergodox_ez.h
 *  - keyboards/ergodox_ez/matrix.c
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

#pragma once

#include <Arduino.h>

#include "kaleidoscope/driver/BaseKeyScannerDescription.h"

#define MCP23018_KEYSCANNER_DESCRIPTION(ROWS_, COLS_) \
  KEYSCANNER_DESCRIPTION(ROWS_, COLS_);               \
  static constexpr uint8_t row_offset = 0;            \
  static constexpr uint8_t column_offset = 0;

#define MCP23018_KEYSCANNER_DESCRIPTION_DATA(BOARD)         \
  KEYSCANNER_DESCRIPTION_DATA(BOARD);                       \
  constexpr uint8_t BOARD::row_offset;                      \
  constexpr uint8_t BOARD::column_offset;

#define _SD(BOARD) \
  BOARD::KeyScannerDescription_
#define MCP23018_KEYSCANNER_DATA(BOARD)                                                      \
  MCP23018_KEYSCANNER_DESCRIPTION_DATA(BOARD ## Description)                                 \
  template<>                                                                               \
  volatile uint16_t BOARD::previousKeyState_[_SD(BOARD)::matrix_rows] = {}; \
  template<>                                                                               \
  volatile uint16_t BOARD::keyState_[_SD(BOARD)::matrix_rows] = {};     \
  template<>                                                                               \
  uint16_t BOARD::masks_[_SD(BOARD)::matrix_rows] = {};                 \
  template<>                                                                               \
  uint8_t BOARD::debounce_matrix_[_SD(BOARD)::matrix_rows][_SD(BOARD)::matrix_columns] = {};

namespace kaleidoscope {
namespace hardware {
namespace ez {
namespace ergodox {

struct MCP23018KeyScannerDescription : kaleidoscope::driver::BaseKeyScannerDescription {
  static const uint8_t debounce = 3;

  /*
   * The following two lines tell the scanner to offset the rows and/or columns
   * by a given amount when _acting_ on key events. This is to support setups
   * where the MCP23018-based scanner is just one part of the full picture.
   */
  static constexpr uint8_t row_offset = 0;
  static constexpr uint8_t column_offset = 0;
};

}
}
}
}

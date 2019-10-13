/* -*- mode: c++ -*-
 * BaseKeyScannerDescription -- KeyScannerDescription base class
 * Copyright (C) 2019  Keyboard.io, Inc
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

#include <Arduino.h>
#include "kaleidoscope/MatrixAddr.h"

#define KEYSCANNER_DESCRIPTION(ROWS_, COLS_)                   \
  static constexpr uint8_t matrix_rows = ROWS_;             \
  static constexpr uint8_t matrix_columns = COLS_;          \
  typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;

#define KEYSCANNER_DESCRIPTION_DATA(BOARD)                     \
  const uint8_t BOARD::matrix_rows;                          \
  const uint8_t BOARD::matrix_columns;

namespace kaleidoscope {
namespace driver {

struct BaseKeyScannerDescription {
  static constexpr uint8_t matrix_rows = 0;
  static constexpr uint8_t matrix_columns = 0;

  typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;
};

}
}

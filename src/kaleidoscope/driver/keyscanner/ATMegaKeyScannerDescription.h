/* -*- mode: c++ -*-
 * ATMegaKeyScannerDescription -- ATMega-based keyscanner component, description
 * Copyright (C) 2018-2019  Keyboard.io, Inc
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

#include "kaleidoscope/driver/KeyScannerDescription.h"
#include "kaleidoscope/hardware/avr/pins_and_ports.h"
#include "kaleidoscope/macro_helpers.h"

#define ROW_PIN_LIST(...)  __VA_ARGS__
#define COL_PIN_LIST(...)  __VA_ARGS__

#define ATMEGA_KEYSCANNER_DESCRIPTION(ROW_PINS_, COL_PINS_)             \
  KEYSCANNER_DESCRIPTION(NUM_ARGS(ROW_PINS_), NUM_ARGS(COL_PINS_));     \
  static constexpr uint8_t matrix_row_pins[matrix_rows] =  ROW_PINS_;   \
  static constexpr uint8_t matrix_col_pins[matrix_columns] =  COL_PINS_;

#define ATMEGA_KEYSCANNER_DESCRIPTION_DATA(BOARD)           \
  KEYSCANNER_DESCRIPTION_DATA(BOARD);                       \
  constexpr uint8_t BOARD::matrix_row_pins[matrix_rows];    \
  constexpr uint8_t BOARD::matrix_col_pins[matrix_columns];

#define _SD(BOARD) \
  BOARD::KeyScannerDescription_
#define ATMEGA_KEYSCANNER_DATA(BOARD)                                                      \
  ATMEGA_KEYSCANNER_DESCRIPTION_DATA(BOARD ## Description)                                 \
  template<>                                                                               \
  volatile uint16_t BOARD::previousKeyState_[_SD(BOARD)::matrix_rows] = {}; \
  template<>                                                                               \
  volatile uint16_t BOARD::keyState_[_SD(BOARD)::matrix_rows] = {};     \
  template<>                                                                               \
  uint16_t BOARD::masks_[_SD(BOARD)::matrix_rows] = {};                 \
  template<>                                                                               \
  uint8_t BOARD::debounce_matrix_[_SD(BOARD)::matrix_rows][_SD(BOARD)::matrix_columns] = {}; \
                                                                                           \
  ISR(TIMER1_OVF_vect) {                                                                   \
    KeyboardHardware.readMatrix();                                                         \
  }

namespace kaleidoscope {
namespace driver {
namespace keyscanner {

struct ATMegaKeyScannerDescription : kaleidoscope::driver::KeyScannerDescription {
  static const uint8_t debounce = 3;

  /*
   * The following two lines declare an empty array. Both of these must be
   * shadowed by the descendant keyscanner description class.
   */
  static constexpr uint8_t matrix_row_pins[] = {};
  static constexpr uint8_t matrix_col_pins[] = {};
};

}
}
}

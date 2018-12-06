/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-ExpanderKeyboard -- Base class for some keyboards with expanders
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

#pragma once

#if KALEIDOSCOPE_WITH_EXPANDER_KEYBOARD

#define KALEIDOSCOPE_WITH_ATMEGA_KEYBOARD 1

#include <Arduino.h>
#include <KeyboardioHID.h>
#include "Kaleidoscope-HIDAdaptor-KeyboardioHID.h"
#include "kaleidoscope/hardware/ATMegaKeyboard.h"

#define EXPANDER_KEYBOARD_CONFIG(EXPANDER_ROWS_, ROW_PINS_, COL_PINS_)  \
  ATMEGA_KEYBOARD_CONFIG_EXT(                                           \
      ROW_PIN_LIST(ROW_PINS_),                                          \
      COL_PIN_LIST(COL_PINS_),                                          \
      matrix_row_pin_cnt + EXPANDER_ROWS_);                             \
  static const int8_t matrix_expander_rows = EXPANDER_ROWS_;

#define EXPANDER_KEYBOARD_DATA(BOARD)           \
  ATMEGA_KEYBOARD_DATA(BOARD);                  \
  const int8_t BOARD::matrix_expander_rows;

#define EXPANDER_ROWS(n) n

namespace kaleidoscope {
namespace hardware {
class ExpanderKeyboard : public ATMegaKeyboard {
 public:
  ExpanderKeyboard(void) {}

  void setup(void);
  void readMatrix(void);

 protected:
  // these can be overridden by the subclass
  uint8_t initExpander();
  void reattachExpanderOnError();
  void setupExpander();
  uint16_t readExpanderCols();
  void selectExpanderRow(uint8_t row);

 private:
  static uint8_t expander_error_;
};
}
}

#endif

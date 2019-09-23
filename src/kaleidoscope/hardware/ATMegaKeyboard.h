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

#pragma once

#if KALEIDOSCOPE_WITH_ATMEGA_KEYBOARD

#include <Arduino.h>
#include <KeyboardioHID.h>
#include "Kaleidoscope-HIDAdaptor-KeyboardioHID.h"
#include "kaleidoscope/MatrixAddr.h"

#include "kaleidoscope/macro_helpers.h"
#include "kaleidoscope/hardware/avr/pins_and_ports.h"

#include <avr/wdt.h>

#ifndef CRGB

struct cRGB {
  uint8_t r, g, b;
};

#define CRGB(r,g,b) (cRGB){b, g, r}

#endif

#include "kaleidoscope/Hardware.h"
#include "kaleidoscope/driver/MCU.h"

#define ROW_PIN_LIST(...)  __VA_ARGS__
#define COL_PIN_LIST(...)  __VA_ARGS__

// By implementing all KeyAddr based access methods via macros in
// the derived hardware classes, we deal with the problem that
// keyboard matrix dimension (matrix_rows/matrix_columns)
// and thus type KeyAddr is only known to the derived hardware classes
// but not to ATMegaKeyboard.
//
#define ATMEGA_KEYBOARD_MATRIX_ACCESS_METHODS                            \
  bool isKeyswitchPressed(KeyAddr key_addr) {                            \
    return (bitRead(keyState_[key_addr.row()],                           \
               key_addr.col()) != 0);                                    \
  }                                                                      \
  DEPRECATED(ROW_COL_FUNC) bool isKeyswitchPressed(uint8_t row, byte col)\
  {                                                                      \
    return isKeyswitchPressed(KeyAddr(row, col));                        \
  }                                                                      \
  bool isKeyswitchPressed(uint8_t keyIndex) {                            \
    keyIndex--;                                                          \
    return isKeyswitchPressed(KeyAddr(keyIndex));                        \
  }                                                                      \
  bool wasKeyswitchPressed(KeyAddr key_addr) {                           \
    return (bitRead(previousKeyState_[key_addr.row()],                   \
               key_addr.col()) != 0);                                    \
  }                                                                      \
  DEPRECATED(ROW_COL_FUNC) bool wasKeyswitchPressed(uint8_t row, byte col)\
  {                                                                      \
    return wasKeyswitchPressed(KeyAddr(row, col));                       \
  }                                                                      \
  bool wasKeyswitchPressed(uint8_t keyIndex) {                           \
    keyIndex--;                                                          \
    return wasKeyswitchPressed(KeyAddr(keyIndex));                       \
  }                                                                      \
  void maskKey(KeyAddr key_addr) {                                       \
    if (!key_addr.isValid())                                             \
      return;                                                            \
                                                                         \
    bitWrite(masks_[key_addr.row()], key_addr.col(), 1);                 \
  }                                                                      \
  DEPRECATED(ROW_COL_FUNC) void maskKey(byte row, byte col) {            \
    maskKey(KeyAddr(row, col));                                          \
  }                                                                      \
  void unMaskKey(KeyAddr key_addr) {                                     \
    if (!key_addr.isValid())                                             \
      return;                                                            \
                                                                         \
    bitWrite(masks_[key_addr.row()], key_addr.col(), 0);                 \
  }                                                                      \
  DEPRECATED(ROW_COL_FUNC) void unMaskKey(byte row, byte col) {          \
    unMaskKey(KeyAddr(row, col));                                        \
  }                                                                      \
  bool isKeyMasked(KeyAddr key_addr) {                                   \
    if (!key_addr.isValid())                                             \
      return false;                                                      \
                                                                         \
    return bitRead(masks_[key_addr.row()],                               \
               key_addr.col());                                          \
  }                                                                      \
  DEPRECATED(ROW_COL_FUNC) bool isKeyMasked(byte row, byte col) {        \
    return isKeyMasked(KeyAddr(row, col));                               \
  }

#define ATMEGA_KEYBOARD_CONFIG(ROW_PINS_, COL_PINS_)          	         \
  static const int8_t matrix_rows = NUM_ARGS(ROW_PINS_);                 \
  static const int8_t matrix_columns = NUM_ARGS(COL_PINS_);              \
  static constexpr uint8_t matrix_row_pins[matrix_rows] =  ROW_PINS_;    \
  static constexpr uint8_t matrix_col_pins[matrix_columns] =  COL_PINS_; \
  typedef MatrixAddr<NUM_ARGS(ROW_PINS_), NUM_ARGS(COL_PINS_)> KeyAddr;  \
                                                                         \
  static uint16_t previousKeyState_[matrix_rows];                        \
  static uint16_t keyState_[matrix_rows];                                \
  static uint16_t masks_[matrix_rows];                                   \
  static uint8_t debounce_matrix_[matrix_rows][matrix_columns];          \
                                                                         \
  ATMEGA_KEYBOARD_MATRIX_ACCESS_METHODS

#define ATMEGA_KEYBOARD_DATA(BOARD)                             \
  const int8_t BOARD::matrix_rows;                              \
  const int8_t BOARD::matrix_columns;                           \
  constexpr uint8_t BOARD::matrix_row_pins[matrix_rows];        \
  constexpr uint8_t BOARD::matrix_col_pins[matrix_columns];     \
  uint16_t BOARD::previousKeyState_[matrix_rows];               \
  uint16_t BOARD::keyState_[matrix_rows];                       \
  uint16_t BOARD::masks_[matrix_rows];                          \
  uint8_t BOARD::debounce_matrix_[matrix_rows][matrix_columns]; \
                                                                \
  ISR(TIMER1_OVF_vect) {                                        \
    BOARD::do_scan_ = true;                                     \
  }

namespace kaleidoscope {
namespace hardware {
class ATMegaKeyboard : public kaleidoscope::Hardware {
 public:
  ATMegaKeyboard(void) {}

  static uint8_t debounce;

  void setup(void);
  void readMatrix(void);
  void actOnMatrixScan();
  void scanMatrix();

  uint8_t pressedKeyswitchCount();

  uint8_t previousPressedKeyswitchCount();

  static bool do_scan_;

 protected:
  kaleidoscope::driver::mcu::ATMega32U4 mcu_;

 private:

  uint16_t readCols();
  uint16_t debounceMaskForRow(uint8_t row);
  void debounceRow(uint16_t change, uint8_t row);
};
}
}

#endif

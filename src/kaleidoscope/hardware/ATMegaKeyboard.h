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

#include "kaleidoscope/macro_helpers.h"
#include "kaleidoscope/key_events.h"
#include "kaleidoscope/hardware/avr/pins_and_ports.h"

#include <avr/wdt.h>

struct cRGB {
  uint8_t r, g, b;
};

#define CRGB(r,g,b) (cRGB){b, g, r}

#define ROW_PIN_LIST(...)  __VA_ARGS__
#define COL_PIN_LIST(...)  __VA_ARGS__

#define ATMEGA_KEYBOARD_CONFIG(ROW_PINS_, COL_PINS_)          	         \
  static const int8_t matrix_rows = NUM_ARGS(ROW_PINS_);                 \
  static const int8_t matrix_columns = NUM_ARGS(COL_PINS_);              \
  static constexpr uint8_t matrix_row_pins[matrix_rows] =  ROW_PINS_;    \
  static constexpr uint8_t matrix_col_pins[matrix_columns] =  COL_PINS_; \
                                                                         \
  static uint16_t previousKeyState_[matrix_rows];                        \
  static uint16_t keyState_[matrix_rows];                                \
  static uint16_t masks_[matrix_rows];                                   \
  static uint8_t debounce_matrix_[matrix_rows][matrix_columns];

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
class ATMegaKeyboard {
 public:
  ATMegaKeyboard(void) {}

  // these will be overridden by the subclass
  static uint8_t debounce;

  void syncLeds(void) {}
  void setCrgbAt(uint8_t row, byte col, cRGB color) {}
  void setCrgbAt(int8_t i, cRGB crgb) {}
  cRGB getCrgbAt(int8_t i) {
    return CRGB(0, 0, 0);
  }
  int8_t getLedIndex(uint8_t row, byte col) {
    return -1;
  }

  void setup(void);

  /** Detaching from / attaching to the host.
   *
   * These two functions should detach the device from (or attach it to) the
   * host, preferably without rebooting the device. Their purpose is to allow
   * one to do some configuration inbetween, so the re-attach happens with
   * different properties. The device remains powered between these operations,
   * only the connection to the host gets severed.
   */
  void detachFromHost();
  void attachToHost();

  void readMatrix(void);

  /**
   * Check the number of key switches currently pressed.
   *
   * @returns the number of keys pressed.
   */
  uint8_t pressedKeyswitchCount();

  /** Key switch states
   *
   * These methods offer a way to peek at the key switch states, for those cases
   * where we need to deal with the state closest to the hardware. Some methods
   * offer a way to check if a key is pressed, others return the number of
   * pressed keys.
   */
  /**
   * Check if a key is pressed at a given position.
   *
   * @param row is the row the key is located at in the matrix.
   * @param col is the column the key is located at in the matrix.
   *
   * @returns true if the key is pressed, false otherwise.
   */
  bool isKeyswitchPressed(uint8_t row, byte col);

  /**
   * Check if a key is pressed at a given position.
   *
   * @param keyIndex is the key index, as calculated by `keyIndex`.
   *
   * @note Key indexes start at 1, not 0!
   *
   * @returns true if the key is pressed, false otherwise.
   */
  bool isKeyswitchPressed(uint8_t keyIndex);

  void actOnMatrixScan();
  void scanMatrix();

  /* Key masking
   * -----------
   *
   * There are situations when one wants to ignore key events for a while, and
   * mask them out. These functions help do that. In isolation, they do nothing,
   * plugins and the core firmware is expected to make use of these.
   *
   * See `handleKeyswitchEvent` in the Kaleidoscope sources for a use-case.
   */
  void maskKey(byte row, byte col);
  void unMaskKey(byte row, byte col);
  bool isKeyMasked(byte row, byte col);

  static bool do_scan_;

 private:

  uint16_t readCols();
  uint16_t debounceMaskForRow(uint8_t row);
  void debounceRow(uint16_t change, uint8_t row);
};
}
}

#endif

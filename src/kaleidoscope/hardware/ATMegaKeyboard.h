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
    KeyboardHardware.do_scan_ = true;                           \
  }

namespace kaleidoscope {
namespace hardware {
template<typename DerivedKeyboard__>
class ATMegaKeyboard : public kaleidoscope::Hardware {
 public:
  ATMegaKeyboard(void) {}

  void setup(void) {
    wdt_disable();

    for (int8_t i = 0; i < derivedKeyboard_().matrix_columns; i++) {
      DDR_INPUT(derivedKeyboard_().matrix_col_pins[i]);
      ENABLE_PULLUP(derivedKeyboard_().matrix_col_pins[i]);
    }

    for (int8_t i = 0; i < derivedKeyboard_().matrix_rows; i++) {
      DDR_OUTPUT(derivedKeyboard_().matrix_row_pins[i]);
      OUTPUT_HIGH(derivedKeyboard_().matrix_row_pins[i]);
    }

    /* Set up Timer1 for 1700usec */
    TCCR1B = _BV(WGM13);
    TCCR1A = 0;

    const uint32_t cycles = (F_CPU / 2000000) * 1700;

    ICR1 = cycles;
    TCCR1B = _BV(WGM13) | _BV(CS10);
    TIMSK1 = _BV(TOIE1);
  }
  void __attribute__((optimize(3))) readMatrix(void) {
    for (int8_t current_row = 0; current_row < derivedKeyboard_().matrix_rows; current_row++) {
      uint16_t mask, cols;

      mask = derivedKeyboard_().debounceMaskForRow(current_row);

      OUTPUT_TOGGLE(derivedKeyboard_().matrix_row_pins[current_row]);
      cols = (derivedKeyboard_().readCols() & mask) | (derivedKeyboard_().keyState_[current_row] & ~mask);
      OUTPUT_TOGGLE(derivedKeyboard_().matrix_row_pins[current_row]);
      derivedKeyboard_().debounceRow(cols ^ derivedKeyboard_().keyState_[current_row], current_row);
      derivedKeyboard_().keyState_[current_row] = cols;
    }
  }
  void __attribute__((optimize(3))) actOnMatrixScan() {
    for (int8_t row = 0; row < derivedKeyboard_().matrix_rows; row++) {
      for (int8_t col = 0; col < derivedKeyboard_().matrix_columns; col++) {
        uint8_t keyState = (bitRead(derivedKeyboard_().previousKeyState_[row], col) << 0) |
                           (bitRead(derivedKeyboard_().keyState_[row], col) << 1);
        if (keyState) {
          handleKeyswitchEvent(Key_NoKey, row, col, keyState);
        }
      }
      derivedKeyboard_().previousKeyState_[row] = derivedKeyboard_().keyState_[row];
    }
  }
  void scanMatrix() {
    if (do_scan_) {
      do_scan_ = false;
      // We only want to update our matrix if the timer has expired.
      derivedKeyboard_().readMatrix();
    }
    // We ALWAYS want to tell Kaleidoscope about the state of the matrix
    derivedKeyboard_().actOnMatrixScan();
  }

  uint8_t pressedKeyswitchCount() {
    uint8_t count = 0;

    for (int8_t r = 0; r < derivedKeyboard_().matrix_rows; r++) {
      count += __builtin_popcount(derivedKeyboard_().keyState_[r]);
    }
    return count;
  }
  bool isKeyswitchPressed(uint8_t row, byte col) {
    return (bitRead(derivedKeyboard_().keyState_[row], col) != 0);
  }
  bool isKeyswitchPressed(uint8_t keyIndex) {
    keyIndex--;
    return isKeyswitchPressed(keyIndex / derivedKeyboard_().matrix_columns,
                              keyIndex % derivedKeyboard_().matrix_columns);
  }

  uint8_t previousPressedKeyswitchCount() {
    uint8_t count = 0;

    for (int8_t r = 0; r < derivedKeyboard_().matrix_rows; r++) {
      count += __builtin_popcount(derivedKeyboard_().previousKeyState_[r]);
    }
    return count;
  }
  bool wasKeyswitchPressed(uint8_t row, byte col) {
    return (bitRead(derivedKeyboard_().previousKeyState_[row], col) != 0);
  }
  bool wasKeyswitchPressed(uint8_t keyIndex) {
    keyIndex--;
    return wasKeyswitchPressed(keyIndex / derivedKeyboard_().matrix_columns,
                               keyIndex % derivedKeyboard_().matrix_columns);
  }

  void maskKey(byte row, byte col) {
    if (row >= derivedKeyboard_().matrix_rows || col >= derivedKeyboard_().matrix_columns)
      return;

    bitWrite(derivedKeyboard_().masks_[row], col, 1);
  }
  void unMaskKey(byte row, byte col) {
    if (row >= derivedKeyboard_().matrix_rows || col >= derivedKeyboard_().matrix_columns)
      return;

    bitWrite(derivedKeyboard_().masks_[row], col, 0);

  }
  bool isKeyMasked(byte row, byte col) {
    if (row >= derivedKeyboard_().matrix_rows || col >= derivedKeyboard_().matrix_columns)
      return false;

    return bitRead(derivedKeyboard_().masks_[row], col);
  }

  void resetDevice() {
    derivedKeyboard_().bootloader_.resetDevice();
  }

  uint8_t debounce;
  bool do_scan_;

 protected:
  kaleidoscope::driver::mcu::ATMega32U4 mcu_;
  DerivedKeyboard__ &derivedKeyboard_() {
    return static_cast<DerivedKeyboard__ &>(*this);
  }

 private:
  /*
   * This function has loop unrolling disabled on purpose: we want to give the
   * hardware enough time to produce stable PIN reads for us. If we unroll the
   * loop, we will not have that, because even with the NOP, the codepath is too
   * fast. If we don't have stable reads, then entire rows or columns will behave
   * erratically.
   *
   * For this reason, we ask the compiler to not unroll our loop, which in turn,
   * gives hardware enough time to produce stable reads, at the cost of a little
   * bit of speed.
   *
   * Do not remove the attribute!
   */
  uint16_t __attribute__((optimize("no-unroll-loops"))) readCols() {
    uint16_t results = 0x00 ;
    for (int8_t i = 0; i < derivedKeyboard_().matrix_columns; i++) {
      asm("NOP"); // We need to pause a beat before reading or we may read before the pin is hot
      results |= (!READ_PIN(derivedKeyboard_().matrix_col_pins[i]) << i);
    }
    return results;
  }
  uint16_t debounceMaskForRow(uint8_t row) {
    uint16_t result = 0;

    for (int16_t c = 0; c < derivedKeyboard_().matrix_columns; ++c) {
      if (derivedKeyboard_().debounce_matrix_[row][c]) {
        --derivedKeyboard_().debounce_matrix_[row][c];
      } else {
        result |= _BV(c);
      }
    }
    return result;
  }
  void debounceRow(uint16_t change, uint8_t row) {
    for (int16_t i = 0; i < derivedKeyboard_().matrix_columns; ++i) {
      if (change & _BV(i)) {
        derivedKeyboard_().debounce_matrix_[row][i] = debounce;
      }
    }
  }
};
}
}

#endif

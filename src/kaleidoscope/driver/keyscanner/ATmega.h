/* -*- mode: c++ -*-
 * kaleidoscope::driver::keyscanner::ATmega -- AVR ATmega-based keyscanner component
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

#include "kaleidoscope/macro_helpers.h"
#include "kaleidoscope/driver/keyscanner/Base.h"
#include "kaleidoscope/driver/keyscanner/None.h"

#include "kaleidoscope/device/avr/pins_and_ports.h"

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
#include <avr/wdt.h>
#endif // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

#define ROW_PIN_LIST(...)  __VA_ARGS__
#define COL_PIN_LIST(...)  __VA_ARGS__

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
#define ATMEGA_KEYSCANNER_PROPS(ROW_PINS_, COL_PINS_)                   \
  KEYSCANNER_PROPS(NUM_ARGS(ROW_PINS_), NUM_ARGS(COL_PINS_));           \
  static constexpr uint8_t matrix_row_pins[matrix_rows] =  ROW_PINS_;   \
  static constexpr uint8_t matrix_col_pins[matrix_columns] =  COL_PINS_;
#else // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
#define ATMEGA_KEYSCANNER_PROPS(ROW_PINS_, COL_PINS_)               \
  KEYSCANNER_PROPS(NUM_ARGS(ROW_PINS_), NUM_ARGS(COL_PINS_));
#endif // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

#define ATMEGA_KEYSCANNER_BOILERPLATE                                                                   \
  const uint8_t kaleidoscope::Device::KeyScannerProps::matrix_rows;                 \
  const uint8_t kaleidoscope::Device::KeyScannerProps::matrix_columns; \
  constexpr uint8_t kaleidoscope::Device::KeyScannerProps::matrix_row_pins[matrix_rows];                \
  constexpr uint8_t kaleidoscope::Device::KeyScannerProps::matrix_col_pins[matrix_columns];             \
  template<>                                                                                            \
  uint16_t kaleidoscope::Device::KeyScanner::previousKeyState_[kaleidoscope::Device::KeyScannerProps::matrix_rows] = {}; \
  template<>                                                                                            \
  uint16_t kaleidoscope::Device::KeyScanner::keyState_[kaleidoscope::Device::KeyScannerProps::matrix_rows] = {};         \
  template<>                                                                                            \
  uint16_t kaleidoscope::Device::KeyScanner::masks_[kaleidoscope::Device::KeyScannerProps::matrix_rows] = {};            \
  template<>                                                                               \
  uint8_t kaleidoscope::Device::KeyScanner::debounce_matrix_[kaleidoscope::Device::KeyScannerProps::matrix_rows][kaleidoscope::Device::KeyScannerProps::matrix_columns] = {}; \
                                                                                           \
  ISR(TIMER1_OVF_vect) {                                                                   \
    Runtime.device().keyScanner().do_scan_ = true;                                         \
  }

namespace kaleidoscope {
namespace driver {
namespace keyscanner {

struct ATmegaProps: kaleidoscope::driver::keyscanner::BaseProps {
  static const uint8_t debounce = 3;

  /*
   * The following two lines declare an empty array. Both of these must be
   * shadowed by the descendant keyscanner description class.
   */
  static constexpr uint8_t matrix_row_pins[] = {};
  static constexpr uint8_t matrix_col_pins[] = {};
};


#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
template <typename _KeyScannerProps>
class ATmega: public kaleidoscope::driver::keyscanner::Base<_KeyScannerProps> {
 private:
  typedef ATmega<_KeyScannerProps> ThisType;

 public:
  void setup() {
    static_assert(
      sizeof(_KeyScannerProps::matrix_row_pins) > 0,
      "The key scanner description has an empty array of matrix row pins."
    );
    static_assert(
      sizeof(_KeyScannerProps::matrix_col_pins) > 0,
      "The key scanner description has an empty array of matrix column pins."
    );

    wdt_disable();

    for (uint8_t i = 0; i < _KeyScannerProps::matrix_columns; i++) {
      DDR_INPUT(_KeyScannerProps::matrix_col_pins[i]);
      ENABLE_PULLUP(_KeyScannerProps::matrix_col_pins[i]);
    }

    for (uint8_t i = 0; i < _KeyScannerProps::matrix_rows; i++) {
      DDR_OUTPUT(_KeyScannerProps::matrix_row_pins[i]);
      OUTPUT_HIGH(_KeyScannerProps::matrix_row_pins[i]);
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
    for (uint8_t current_row = 0; current_row < _KeyScannerProps::matrix_rows; current_row++) {
      uint16_t mask, cols;

      mask = debounceMaskForRow(current_row);

      OUTPUT_TOGGLE(_KeyScannerProps::matrix_row_pins[current_row]);
      cols = (readCols() & mask) | (keyState_[current_row] & ~mask);
      OUTPUT_TOGGLE(_KeyScannerProps::matrix_row_pins[current_row]);
      debounceRow(cols ^ keyState_[current_row], current_row);
      keyState_[current_row] = cols;
    }
  }

  void scanMatrix() {
    if (do_scan_) {
      do_scan_ = false;
      readMatrix();
    }
    actOnMatrixScan();
  }

  void __attribute__((optimize(3))) actOnMatrixScan() {
    for (byte row = 0; row < _KeyScannerProps::matrix_rows; row++) {
      for (byte col = 0; col < _KeyScannerProps::matrix_columns; col++) {
        uint8_t keyState = (bitRead(previousKeyState_[row], col) << 0) |
                           (bitRead(keyState_[row], col) << 1);
        if (keyState) {
          ThisType::handleKeyswitchEvent(Key_NoKey, typename _KeyScannerProps::KeyAddr(row, col), keyState);
        }
      }
      previousKeyState_[row] = keyState_[row];
    }
  }

  uint8_t pressedKeyswitchCount() {
    uint8_t count = 0;

    for (int8_t r = 0; r < _KeyScannerProps::matrix_rows; r++) {
      count += __builtin_popcount(keyState_[r]);
    }
    return count;
  }
  bool isKeyswitchPressed(typename _KeyScannerProps::KeyAddr key_addr) {
    return (bitRead(keyState_[key_addr.row()],
                    key_addr.col()) != 0);
  }

  uint8_t previousPressedKeyswitchCount() {
    uint8_t count = 0;

    for (int8_t r = 0; r < _KeyScannerProps::matrix_rows; r++) {
      count += __builtin_popcount(previousKeyState_[r]);
    }
    return count;
  }
  bool wasKeyswitchPressed(typename _KeyScannerProps::KeyAddr key_addr) {
    return (bitRead(previousKeyState_[key_addr.row()],
                    key_addr.col()) != 0);
  }

  void maskKey(typename _KeyScannerProps::KeyAddr key_addr) {
    if (!key_addr.isValid())
      return;

    bitWrite(masks_[key_addr.row()], key_addr.col(), 1);
  }
  void unMaskKey(typename _KeyScannerProps::KeyAddr key_addr) {
    if (!key_addr.isValid())
      return;

    bitWrite(masks_[key_addr.row()], key_addr.col(), 0);
  }
  bool isKeyMasked(typename _KeyScannerProps::KeyAddr key_addr) {
    if (!key_addr.isValid())
      return false;

    return bitRead(masks_[key_addr.row()],
                   key_addr.col());
  }

  bool do_scan_;

 private:
  typedef _KeyScannerProps KeyScannerProps_;
  static uint16_t previousKeyState_[_KeyScannerProps::matrix_rows];
  static uint16_t keyState_[_KeyScannerProps::matrix_rows];
  static uint16_t masks_[_KeyScannerProps::matrix_rows];
  static uint8_t debounce_matrix_[_KeyScannerProps::matrix_rows][_KeyScannerProps::matrix_columns];

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
  __attribute__((optimize("no-unroll-loops")))
  uint16_t readCols() {
    uint16_t results = 0x00 ;
    for (uint8_t i = 0; i < _KeyScannerProps::matrix_columns; i++) {
      asm("NOP"); // We need to pause a beat before reading or we may read before the pin is hot
      results |= (!READ_PIN(_KeyScannerProps::matrix_col_pins[i]) << i);
    }
    return results;
  }

  uint16_t debounceMaskForRow(uint8_t row) {
    uint16_t result = 0;

    for (uint16_t c = 0; c < _KeyScannerProps::matrix_columns; ++c) {
      if (debounce_matrix_[row][c]) {
        --debounce_matrix_[row][c];
      } else {
        result |= _BV(c);
      }
    }
    return result;
  }

  void debounceRow(uint16_t change, uint8_t row) {
    for (uint16_t i = 0; i < _KeyScannerProps::matrix_columns; ++i) {
      if (change & _BV(i)) {
        debounce_matrix_[row][i] = _KeyScannerProps::debounce;
      }
    }
  }
};
#else // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
template <typename _KeyScannerProps>
class ATmega : public keyscanner::None {};
#endif // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

}
}
}

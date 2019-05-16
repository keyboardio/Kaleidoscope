/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-SOFTHRUF-Splitography -- Splitography hardware support for Kaleidoscope
 * Copyright (C) 2018  Keyboard.io, Inc
 *
 * Based on QMK (commit e9a67f8fd) and sdothum's fork (commit 8616b44)
 *  (C) Jack Humbert, Jun Wako, Steven Hum, and others
 * Original QMK sources:
 *  - keyboards/splitography/config.h
 *  - keyboards/splitography/splitography.h
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

#ifdef ARDUINO_AVR_SPLITOGRAPHY

#include <Arduino.h>
#define HARDWARE_IMPLEMENTATION kaleidoscope::hardware::softhruf::Splitography
#include "Kaleidoscope-HIDAdaptor-KeyboardioHID.h"

#include "kaleidoscope/macro_helpers.h"
#include "kaleidoscope/hardware/avr/pins_and_ports.h"

#include "kaleidoscope/hardware/ATMegaKeyboard.h"

namespace kaleidoscope {
namespace hardware {
namespace softhruf {
class Splitography: public kaleidoscope::hardware::ATMegaKeyboard {
 public:
  Splitography(void) {
    mcu_.disableJTAG();
  }

  ATMEGA_KEYBOARD_CONFIG(
    ROW_PIN_LIST({ PIN_D0, PIN_D1, PIN_D2, PIN_D3 }),
    COL_PIN_LIST({ PIN_F0, PIN_F1, PIN_F4, PIN_F5, PIN_F6, PIN_F7, PIN_C7, PIN_C6, PIN_B6, PIN_B5, PIN_B4, PIN_D7 })
  );

  static constexpr int8_t led_count = 0;

  typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;
  static constexpr int8_t numKeys() {
    return matrix_columns * matrix_rows;
  }
};

#define PER_KEY_DATA(dflt,                                                       \
      r0c0 ,r0c1 ,r0c2 ,r0c3 ,r0c4 ,r0c5   ,r0c6 ,r0c7 ,r0c8 ,r0c9 ,r0c10 ,r0c11   \
     ,r1c0 ,r1c1 ,r1c2 ,r1c3 ,r1c4 ,r1c5   ,r1c6 ,r1c7 ,r1c8 ,r1c9 ,r1c10 ,r1c11   \
     ,r2c0 ,r2c1 ,r2c2 ,r2c3 ,r2c4 ,r2c5   ,r2c6 ,r2c7 ,r2c8 ,r2c9 ,r2c10 ,r2c11   \
                             ,r3c4 ,r3c5   ,r3c6 ,r3c7                             \
  )                                                                                \
                                                                                   \
      r0c0 ,r0c1 ,r0c2 ,r0c3 ,r0c4 ,r0c5 , r0c6 ,r0c7 ,r0c8 ,r0c9 ,r0c10 ,r0c11,   \
      r1c0 ,r1c1 ,r1c2 ,r1c3 ,r1c4 ,r1c5 , r1c6 ,r1c7 ,r1c8 ,r1c9 ,r1c10 ,r1c11,   \
      r2c0 ,r2c1 ,r2c2 ,r2c3 ,r2c4 ,r2c5 , r2c6 ,r2c7 ,r2c8 ,r2c9 ,r2c10 ,r2c11,   \
      dflt ,dflt ,dflt ,dflt ,r3c4 ,r3c5  ,r3c6 ,r3c7 ,dflt ,dflt ,dflt  ,dflt

#define PER_KEY_DATA_STACKED(dflt,                                           \
      r0c0 ,r0c1 ,r0c2 ,r0c3 ,r0c4  ,r0c5                                      \
     ,r1c0 ,r1c1 ,r1c2 ,r1c3 ,r1c4  ,r1c5                                      \
     ,r2c0 ,r2c1 ,r2c2 ,r2c3 ,r2c4  ,r2c5                                      \
                             ,r3c4  ,r3c5                                      \
                                                                               \
     ,r0c6 ,r0c7 ,r0c8 ,r0c9 ,r0c10 ,r0c11                                     \
     ,r1c6 ,r1c7 ,r1c8 ,r1c9 ,r1c10 ,r1c11                                     \
     ,r2c6 ,r2c7 ,r2c8 ,r2c9 ,r2c10 ,r2c11                                     \
     ,r3c6 ,r3c7                                                               \
  )                                                                            \
    r0c0 ,r0c1 ,r0c2 ,r0c3 ,r0c4 ,r0c5 ,r0c6 ,r0c7 ,r0c8 ,r0c9 ,r0c10 ,r0c11,  \
    r1c0 ,r1c1 ,r1c2 ,r1c3 ,r1c4 ,r1c5 ,r1c6 ,r1c7 ,r1c8 ,r1c9 ,r1c10 ,r1c11,  \
    r2c0 ,r2c1 ,r2c2 ,r2c3 ,r2c4 ,r2c5 ,r2c6 ,r2c7 ,r2c8 ,r2c9 ,r2c10 ,r2c11,  \
    dflt ,dflt ,dflt ,dflt ,r3c4 ,r3c5 ,r3c6 ,r3c7 ,dflt ,dflt ,dflt  ,dflt

}
}
}

#include "kaleidoscope/hardware/key_indexes.h"

extern kaleidoscope::hardware::softhruf::Splitography &Splitography;

#endif

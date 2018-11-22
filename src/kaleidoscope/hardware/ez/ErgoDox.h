/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-EZ-ErgoDox -- ErgoDox hardware support for Kaleidoscope
 * Copyright (C) 2018  Keyboard.io, Inc
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

#ifdef ARDUINO_AVR_ERGODOX

#include <Arduino.h>

#define HARDWARE_IMPLEMENTATION kaleidoscope::hardware::ez::ErgoDox
#include "Kaleidoscope-HIDAdaptor-KeyboardioHID.h"
#include "kaleidoscope/hardware/ExpanderKeyboard.h"
#include "kaleidoscope/macro_helpers.h"

namespace kaleidoscope {
namespace hardware {
namespace ez {

class ErgoDox : public kaleidoscope::hardware::ExpanderKeyboard {
  friend class ATMegaKeyboard;
  friend class ExpanderKeyboard;
 public:
  ErgoDox(void) {}

  EXPANDER_KEYBOARD_CONFIG(
    EXPANDER_ROWS(7),
    ROW_PIN_LIST({ PIN_B0, PIN_B1, PIN_B2, PIN_B3, PIN_D2, PIN_D3, PIN_C6 }),
    COL_PIN_LIST({ PIN_F0, PIN_F1, PIN_F4, PIN_F5, PIN_F6, PIN_F7 })
  );

  void setup();

  static constexpr int8_t led_count = 0;

  // ErgoDox-specific stuff
  void setStatusLED(uint8_t led, bool state = true);
  void setStatusLEDBrightness(uint8_t led, uint8_t brightness);

  void resetDevice();
};

#define KEYMAP_STACKED(                                                 \
    /* left hand, spatial positions */                                  \
    r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6,                           \
    r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6,                           \
    r2c0, r2c1, r2c2, r2c3, r2c4, r2c5,                                 \
    r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6,                           \
    r4c0, r4c1, r4c2, r4c3, r4c4,                                       \
                            r5c5, r5c6,                                 \
                                  r5c4,                                 \
                      r5c3, r5c2, r5c1,                                 \
                                                                        \
    /* right hand, spatial positions */                                 \
    r0c7,  r0c8,  r0c9,  r0c10, r0c11, r0c12, r0c13,                    \
    r1c7,  r1c8,  r1c9,  r1c10, r1c11, r1c12, r1c13,                    \
           r2c8,  r2c9,  r2c10, r2c11, r2c12, r2c13,                    \
    r3c7,  r3c8,  r3c9,  r3c10, r3c11, r3c12, r3c13,                    \
    r4c9,  r4c10, r4c11, r4c12, r4c13,                                  \
    r5c7,  r5c8,                                                        \
    r5c9,                                                               \
    r5c12, r5c11, r5c10 )                                               \
                                                                        \
  /* matrix positions */                                                \
  {                                                                     \
    { r0c0, r1c0, r2c0, r3c0, r4c0, XXX  },                             \
    { r0c1, r1c1, r2c1, r3c1, r4c1, r5c1 },                             \
    { r0c2, r1c2, r2c2, r3c2, r4c2, r5c2 },                             \
    { r0c3, r1c3, r2c3, r3c3, r4c3, r5c3 },                             \
    { r0c4, r1c4, r2c4, r3c4, r4c4, r5c4 },                             \
    { r0c5, r1c5, r2c5, r3c5, XXX,  r5c5 },                             \
    { r0c6, r1c6, XXX,  r3c6, XXX,  r5c6 },                             \
                                                                        \
    { r0c7,  r1c7,  XXX,   r3c7,  XXX,   r5c7  },                       \
    { r0c8,  r1c8,  r2c8,  r3c8,  XXX,   r5c8  },                       \
    { r0c9,  r1c9,  r2c9,  r3c9,  r4c9,  r5c9  },                       \
    { r0c10, r1c10, r2c10, r3c10, r4c10, r5c10 },                       \
    { r0c11, r1c11, r2c11, r3c11, r4c11, r5c11 },                       \
    { r0c12, r1c12, r2c12, r3c12, r4c12, r5c12 },                       \
    { r0c13, r1c13, r2c13, r3c13, r4c13, XXX   }                        \
  }
}
}
}

#include "kaleidoscope/hardware/key_indexes.h"

extern kaleidoscope::hardware::ez::ErgoDox &ErgoDox;

#endif

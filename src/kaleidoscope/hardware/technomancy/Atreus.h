/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Technomancy-Atreus -- Atreus hardware support for Kaleidoscope
 * Copyright (C) 2018  Keyboard.io, Inc
 *
 * Based on QMK (commit e9a67f8fd)
 *  (C) Jack Humbert, Jun Wako, Phil Hagelberg, and others
 * Original QMK sources:
 *  - keyboards/atreus/atreus.h
 *  - keyboards/atreus/config.h
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

#ifdef ARDUINO_AVR_ATREUS

#include <Arduino.h>
#define HARDWARE_IMPLEMENTATION kaleidoscope::hardware::technomancy::Atreus
#include "Kaleidoscope-HIDAdaptor-KeyboardioHID.h"

#include "kaleidoscope/macro_helpers.h"

#include "kaleidoscope/hardware/ATMegaKeyboard.h"

namespace kaleidoscope {
namespace hardware {
namespace technomancy {
class Atreus: public kaleidoscope::hardware::ATMegaKeyboard {
  friend class ATMegaKeyboard;
 public:
  Atreus(void) {}

#ifdef KALEIDOSCOPE_HARDWARE_ATREUS_PINOUT_ASTAR
  ATMEGA_KEYBOARD_CONFIG(
    ROW_PIN_LIST({PIN_D0, PIN_D1, PIN_D3, PIN_D2}),
    COL_PIN_LIST({PIN_D7, PIN_C6, PIN_B5, PIN_B4, PIN_E6, PIN_D4, PIN_B6, PIN_F6, PIN_F7, PIN_D6, PIN_B7})
  );
#endif

#ifdef KALEIDOSCOPE_HARDWARE_ATREUS_PINOUT_ASTAR_DOWN
  ATMEGA_KEYBOARD_CONFIG(
    ROW_PIN_LIST({PIN_D0, PIN_D1, PIN_D3, PIN_D2}),
    COL_PIN_LIST({PIN_B7, PIN_D6, PIN_F7, PIN_F6, PIN_B6, PIN_D4, PIN_E6, PIN_B4, PIN_B5, PIN_C6, PIN_D7})
  );
#endif

#ifdef KALEIDOSCOPE_HARDWARE_ATREUS_PINOUT_LEGACY_TEENSY2
  ATMEGA_KEYBOARD_CONFIG(
    ROW_PIN_LIST({PIN_D0, PIN_D1, PIN_D2, PIN_D3}),
    COL_PIN_LIST({PIN_F6, PIN_F5, PIN_F4, PIN_B7, PIN_B6, PIN_B5, PIN_B4, PIN_B3, PIN_B2, PIN_B1, PIN_B0})
  );
#endif

  static constexpr int8_t led_count = 0;

  typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;
  typedef MatrixAddr<1, 1> LEDAddr;

  void resetDevice();

 protected:
};

#define KEYMAP(                                                               \
    R0C0, R0C1, R0C2, R0C3, R0C4,             R0C7, R0C8, R0C9, R0C10, R0C11, \
    R1C0, R1C1, R1C2, R1C3, R1C4,             R1C7, R1C8, R1C9, R1C10, R1C11, \
    R2C0, R2C1, R2C2, R2C3, R2C4,             R2C7, R2C8, R2C9, R2C10, R2C11, \
    R3C0, R3C1, R3C2, R3C3, R3C4, R3C5, R3C6, R3C7, R3C8, R3C9, R3C10, R3C11  \
  )                                                                           \
  {                                                                           \
    { R0C0, R0C1, R0C2, R0C3, R0C4, XXX,    R0C7, R0C8, R0C9, R0C10, R0C11 }, \
    { R1C0, R1C1, R1C2, R1C3, R1C4, XXX,    R1C7, R1C8, R1C9, R1C10, R1C11 }, \
    { R2C0, R2C1, R2C2, R2C3, R2C4, R3C5,   R2C7, R2C8, R2C9, R2C10, R2C11 }, \
    { R3C0, R3C1, R3C2, R3C3, R3C4, R3C6,   R3C7, R3C8, R3C9, R3C10, R3C11 }  \
  }

#define KEYMAP_STACKED(                                                       \
    R0C0, R0C1, R0C2, R0C3, R0C4,                                             \
    R1C0, R1C1, R1C2, R1C3, R1C4,                                             \
    R2C0, R2C1, R2C2, R2C3, R2C4,                                             \
    R3C0, R3C1, R3C2, R3C3, R3C4, R3C5,                                       \
                                                                              \
          R0C7, R0C8, R0C9, R0C10, R0C11,                                     \
          R1C7, R1C8, R1C9, R1C10, R1C11,                                     \
          R2C7, R2C8, R2C9, R2C10, R2C11,                                     \
    R3C6, R3C7, R3C8, R3C9, R3C10, R3C11                                      \
  )                                                                           \
  {                                                                           \
    { R0C0, R0C1, R0C2, R0C3, R0C4, XXX,    R0C7, R0C8, R0C9, R0C10, R0C11 }, \
    { R1C0, R1C1, R1C2, R1C3, R1C4, XXX,    R1C7, R1C8, R1C9, R1C10, R1C11 }, \
    { R2C0, R2C1, R2C2, R2C3, R2C4, R3C5,   R2C7, R2C8, R2C9, R2C10, R2C11 }, \
    { R3C0, R3C1, R3C2, R3C3, R3C4, R3C6,   R3C7, R3C8, R3C9, R3C10, R3C11 }  \
  }
}
}
}

#include "kaleidoscope/hardware/key_indexes.h"

extern kaleidoscope::hardware::technomancy::Atreus &Atreus;

#endif

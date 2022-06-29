/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Technomancy-Atreus -- Atreus hardware support for Kaleidoscope
 * Copyright (C) 2018, 2019, 2020  Keyboard.io, Inc
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

#include "kaleidoscope/device/ATmega32U4Keyboard.h"
#include "kaleidoscope/driver/bootloader/avr/Caterina.h"
#include "kaleidoscope/driver/bootloader/avr/HalfKay.h"
#include "kaleidoscope/driver/keyscanner/ATmega.h"

namespace kaleidoscope {
namespace device {
namespace technomancy {

struct AtreusProps : kaleidoscope::device::ATmega32U4KeyboardProps {
  struct KeyScannerProps : public kaleidoscope::driver::keyscanner::ATmegaProps {

    static constexpr uint8_t matrix_rows    = 4;
    static constexpr uint8_t matrix_columns = 12;
    typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD

#ifdef KALEIDOSCOPE_HARDWARE_ATREUS_PINOUT_ASTAR
    static constexpr uint8_t matrix_row_pins[matrix_rows]    = {PIN_D0, PIN_D1, PIN_D3, PIN_D2};
    static constexpr uint8_t matrix_col_pins[matrix_columns] = {PIN_D7, PIN_C6, PIN_B5, PIN_B4, PIN_E6, PIN_D4, PIN_B6, PIN_F6, PIN_F7, PIN_D6, PIN_B7};
#endif

#ifdef KALEIDOSCOPE_HARDWARE_ATREUS_PINOUT_ASTAR_DOWN
    static constexpr uint8_t matrix_row_pins[matrix_rows]    = {PIN_D0, PIN_D1, PIN_D3, PIN_D2};
    static constexpr uint8_t matrix_col_pins[matrix_columns] = {PIN_B7, PIN_D6, PIN_F7, PIN_F6, PIN_B6, PIN_D4, PIN_E6, PIN_B4, PIN_B5, PIN_C6, PIN_D7};
#endif

#ifdef KALEIDOSCOPE_HARDWARE_ATREUS_PINOUT_LEGACY_TEENSY2
    static constexpr uint8_t matrix_row_pins[matrix_rows]    = {PIN_D0, PIN_D1, PIN_D2, PIN_D3};
    static constexpr uint8_t matrix_col_pins[matrix_columns] = {PIN_F6, PIN_F5, PIN_F4, PIN_B7, PIN_B6, PIN_B5, PIN_B4, PIN_B3, PIN_B2, PIN_B1, PIN_B0};
#endif

#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
  };

  typedef kaleidoscope::driver::keyscanner::ATmega<KeyScannerProps> KeyScanner;

#ifdef KALEIDOSCOPE_HARDWARE_ATREUS_PINOUT_LEGACY_TEENSY2
  typedef kaleidoscope::driver::bootloader::avr::HalfKay Bootloader;
#else
  typedef kaleidoscope::driver::bootloader::avr::Caterina Bootloader;
#endif
  static constexpr const char *short_name = "atreus";
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
class Atreus : public kaleidoscope::device::ATmega32U4Keyboard<AtreusProps> {};
#else   // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
/* Device definition omitted for virtual device builds.
 * We need to forward declare the device name, though, as there are
 * some legacy extern references to boards whose definition
 * depends on this.
 */
class Atreus;
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

// clang-format off

#define PER_KEY_DATA(dflt,                                                  \
    R0C0, R0C1, R0C2, R0C3, R0C4,             R0C7, R0C8, R0C9, R0C10, R0C11, \
    R1C0, R1C1, R1C2, R1C3, R1C4,             R1C7, R1C8, R1C9, R1C10, R1C11, \
    R2C0, R2C1, R2C2, R2C3, R2C4,             R2C7, R2C8, R2C9, R2C10, R2C11, \
    R3C0, R3C1, R3C2, R3C3, R3C4, R3C5, R3C6, R3C7, R3C8, R3C9, R3C10, R3C11  \
  )                                                                           \
                                                                              \
    R0C0, R0C1, R0C2, R0C3, R0C4, XXX,    R0C7, R0C8, R0C9, R0C10, R0C11,     \
    R1C0, R1C1, R1C2, R1C3, R1C4, XXX,    R1C7, R1C8, R1C9, R1C10, R1C11,     \
    R2C0, R2C1, R2C2, R2C3, R2C4, R3C5,   R2C7, R2C8, R2C9, R2C10, R2C11,     \
    R3C0, R3C1, R3C2, R3C3, R3C4, R3C6,   R3C7, R3C8, R3C9, R3C10, R3C11

#define PER_KEY_DATA_STACKED(dflt,                                          \
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
    R0C0, R0C1, R0C2, R0C3, R0C4, dflt,   R0C7, R0C8, R0C9, R0C10, R0C11,     \
    R1C0, R1C1, R1C2, R1C3, R1C4, dflt,   R1C7, R1C8, R1C9, R1C10, R1C11,     \
    R2C0, R2C1, R2C2, R2C3, R2C4, R3C5,   R2C7, R2C8, R2C9, R2C10, R2C11,     \
    R3C0, R3C1, R3C2, R3C3, R3C4, R3C6,   R3C7, R3C8, R3C9, R3C10, R3C11

// clang-format on

}  // namespace technomancy
}  // namespace device

EXPORT_DEVICE(kaleidoscope::device::technomancy::Atreus)

}  // namespace kaleidoscope

#endif

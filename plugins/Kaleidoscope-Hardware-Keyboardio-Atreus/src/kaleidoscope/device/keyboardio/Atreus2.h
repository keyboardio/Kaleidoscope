/* -*- mode: c++ -*-
 * Keyboardio Atreus hardware support for Kaleidoscope
 * Copyright (C) 2019  Keyboard.io, Inc
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

#ifdef ARDUINO_AVR_KEYBOARDIO_ATREUS

#include <Arduino.h>

#include "kaleidoscope/device/ATmega32U4Keyboard.h"
#include "kaleidoscope/driver/bootloader/avr/Caterina.h"
#include "kaleidoscope/driver/keyscanner/ATmega.h"

namespace kaleidoscope {
namespace device {
namespace keyboardio {


struct AtreusProps : kaleidoscope::device::ATmega32U4KeyboardProps {
  struct KeyScannerProps : public kaleidoscope::driver::keyscanner::ATmegaProps {


    static constexpr uint8_t matrix_rows    = 4;
    static constexpr uint8_t matrix_columns = 12;
    typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;
#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
    static constexpr uint8_t matrix_row_pins[matrix_rows]    = {PIN_F6, PIN_F5, PIN_F4, PIN_F1};
    static constexpr uint8_t matrix_col_pins[matrix_columns] = {PIN_F7, PIN_E2, PIN_C7, PIN_C6, PIN_B6, PIN_B5, PIN_D7, PIN_D6, PIN_D4, PIN_D5, PIN_D3, PIN_D2};
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
  };

  typedef kaleidoscope::driver::keyscanner::ATmega<KeyScannerProps> KeyScanner;
  typedef kaleidoscope::driver::bootloader::avr::Caterina Bootloader;
  static constexpr const char *short_name = "atreus";
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
class Atreus : public kaleidoscope::device::ATmega32U4Keyboard<AtreusProps> {};
#else  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
/* Device definition omitted for virtual device builds.
 * We need to forward declare the device name, though, as there are
 * some legacy extern references to boards whose definition
 * depends on this.
 */
class Atreus;

#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

// clang-format off

#define PER_KEY_DATA(dflt,                                                    \
    R0C0, R0C1, R0C2, R0C3, R0C4,             R0C7, R0C8, R0C9, R0C10, R0C11, \
    R1C0, R1C1, R1C2, R1C3, R1C4,             R1C7, R1C8, R1C9, R1C10, R1C11, \
    R2C0, R2C1, R2C2, R2C3, R2C4, R2C5, R2C6, R2C7, R2C8, R2C9, R2C10, R2C11, \
    R3C0, R3C1, R3C2, R3C3, R3C4, R3C5, R3C6, R3C7, R3C8, R3C9, R3C10, R3C11  \
  )                                                                           \
  R0C0, R0C1, R0C2, R0C3, R0C4, XXX,  XXX,  R0C7, R0C8, R0C9, R0C10, R0C11, \
  R1C0, R1C1, R1C2, R1C3, R1C4, XXX,  XXX,  R1C7, R1C8, R1C9, R1C10, R1C11, \
  R2C0, R2C1, R2C2, R2C3, R2C4, R2C5, R2C6, R2C7, R2C8, R2C9, R2C10, R2C11, \
  R3C0, R3C1, R3C2, R3C3, R3C4, R3C5, R3C6, R3C7, R3C8, R3C9, R3C10, R3C11

#define PER_KEY_DATA_STACKED(dflt,                                            \
    R0C0, R0C1, R0C2, R0C3, R0C4,                                             \
    R1C0, R1C1, R1C2, R1C3, R1C4,                                             \
    R2C0, R2C1, R2C2, R2C3, R2C4, R2C5,                                       \
    R3C0, R3C1, R3C2, R3C3, R3C4, R3C5,                                       \
                                                                              \
          R0C7, R0C8, R0C9, R0C10, R0C11,                                     \
          R1C7, R1C8, R1C9, R1C10, R1C11,                                     \
    R2C6, R2C7, R2C8, R2C9, R2C10, R2C11,                                     \
    R3C6, R3C7, R3C8, R3C9, R3C10, R3C11                                      \
  )                                                                           \
  R0C0, R0C1, R0C2, R0C3, R0C4, XXX,  XXX,  R0C7, R0C8, R0C9, R0C10, R0C11, \
  R1C0, R1C1, R1C2, R1C3, R1C4, XXX,  XXX,  R1C7, R1C8, R1C9, R1C10, R1C11, \
  R2C0, R2C1, R2C2, R2C3, R2C4, R2C5, R2C6, R2C7, R2C8, R2C9, R2C10, R2C11, \
  R3C0, R3C1, R3C2, R3C3, R3C4, R3C5, R3C6, R3C7, R3C8, R3C9, R3C10, R3C11

// clang-format on

}  // namespace keyboardio
}  // namespace device

EXPORT_DEVICE(kaleidoscope::device::keyboardio::Atreus)

}  // namespace kaleidoscope

#endif

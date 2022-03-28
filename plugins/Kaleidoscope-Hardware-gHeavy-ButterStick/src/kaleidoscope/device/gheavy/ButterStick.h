/* -*- mode: c++ -*-
 * kaleidoscope::device::gheavy::ButterStick -- gHeavy ButterStick hardware support for Kaleidoscope
 * Copyright (C) 2020  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as
 * published by the Free Software Foundation.
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

#ifdef ARDUINO_AVR_GHEAVY_BUTTERSTICK

#define KALEIDOSCOPE_BOOTLOADER_FLIP_WORKAROUND 1

#include <Arduino.h>

#include "kaleidoscope/device/ATmega32U4Keyboard.h"
#include "kaleidoscope/driver/bootloader/avr/FLIP.h"
#include "kaleidoscope/driver/keyscanner/ATmega.h"

namespace kaleidoscope {
namespace device {
namespace gheavy {

struct ButterStickProps : kaleidoscope::device::ATmega32U4KeyboardProps {
  struct KeyScannerProps : public kaleidoscope::driver::keyscanner::ATmegaProps {
    static constexpr uint8_t matrix_rows    = 2;
    static constexpr uint8_t matrix_columns = 10;
    typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;
#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
    static constexpr uint8_t matrix_row_pins[matrix_rows]    = {PIN_F4, PIN_F5};
    static constexpr uint8_t matrix_col_pins[matrix_columns] = {PIN_B0, PIN_B1, PIN_B2, PIN_B3, PIN_B4, PIN_B5, PIN_B6, PIN_B7, PIN_C6, PIN_C7};
#endif  // KALEIDOSCOPE_VIRTUAL_BUILD
  };
  typedef kaleidoscope::driver::keyscanner::ATmega<KeyScannerProps> KeyScanner;
  typedef kaleidoscope::driver::bootloader::avr::FLIP Bootloader;
  static constexpr const char *short_name = "ButterStick";
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
class ButterStick : public kaleidoscope::device::ATmega32U4Keyboard<ButterStickProps> {};
#else  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
/* Device definition omitted for virtual device builds.
 * We need to forward declare the device name, though, as there are
 * some legacy extern references to boards whose definition
 * depends on this.
 */
class ButterStick;

#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

// clang-format off
#define PER_KEY_DATA(dflt,                                           \
         R0C0, R0C1, R0C2, R0C3, R0C4, R0C5, R0C6, R0C7, R0C8, R0C9, \
         R1C0, R1C1, R1C2, R1C3, R1C4, R1C5, R1C6, R1C7, R1C8, R1C9  \
  )                                                                  \
         R0C9, R0C8, R0C7, R0C6, R0C5, R0C4, R0C3, R0C2, R0C1, R0C0, \
         R1C9, R1C8, R1C7, R1C6, R1C5, R1C4, R1C3, R1C2, R1C1, R1C0
// clang-format on

}  // namespace gheavy
}  // namespace device

EXPORT_DEVICE(kaleidoscope::device::gheavy::ButterStick)

}  // namespace kaleidoscope

#endif

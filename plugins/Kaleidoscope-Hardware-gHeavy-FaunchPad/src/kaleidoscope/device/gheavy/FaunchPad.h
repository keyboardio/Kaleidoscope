/* -*- mode: c++ -*-
 * kaleidoscope::device::gheavy::FaunchPad -- gHeavy FaunchPad hardware support for Kaleidoscope
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

#ifdef ARDUINO_AVR_GHEAVY_FAUNCHPAD

#define KALEIDOSCOPE_BOOTLOADER_FLIP_WORKAROUND 1

#include <Arduino.h>

#include "kaleidoscope/device/ATmega32U4Keyboard.h"
#include "kaleidoscope/driver/bootloader/avr/FLIP.h"
#include "kaleidoscope/driver/keyscanner/ATmega.h"

namespace kaleidoscope {
namespace device {
namespace gheavy {

struct FaunchPadProps : kaleidoscope::device::ATmega32U4KeyboardProps {
  struct KeyScannerProps : public kaleidoscope::driver::keyscanner::ATmegaProps {
    static constexpr uint8_t matrix_rows    = 2;
    static constexpr uint8_t matrix_columns = 4;
    typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;
#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
    static constexpr uint8_t matrix_row_pins[matrix_rows]    = {PIN_F4, PIN_F5};
    static constexpr uint8_t matrix_col_pins[matrix_columns] = {PIN_B3, PIN_B2, PIN_B1, PIN_B0};
#endif  // KALEIDOSCOPE_VIRTUAL_BUILD
  };
  typedef kaleidoscope::driver::keyscanner::ATmega<KeyScannerProps> KeyScanner;
  typedef kaleidoscope::driver::bootloader::avr::FLIP Bootloader;
  static constexpr const char *short_name = "FaunchPad";
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
class FaunchPad : public kaleidoscope::device::ATmega32U4Keyboard<FaunchPadProps> {};
#else  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
/* Device definition omitted for virtual device builds.
 * We need to forward declare the device name, though, as there are
 * some legacy extern references to boards whose definition
 * depends on this.
 */
class FaunchPad;

#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

// clang-format off
#define PER_KEY_DATA(dflt,                              \
         R0C0, R0C1, R0C2, R0C3, R0C4, R0C5, R0C6, R0C7 \
  )                                                     \
         R0C0, R0C1, R0C2, R0C3,                        \
         R0C4, R0C5, R0C6, R0C7
// clang-format on

}  // namespace gheavy
}  // namespace device

EXPORT_DEVICE(kaleidoscope::device::gheavy::FaunchPad)

}  // namespace kaleidoscope

#endif

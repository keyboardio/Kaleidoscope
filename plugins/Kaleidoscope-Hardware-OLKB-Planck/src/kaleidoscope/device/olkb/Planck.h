/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-OLKB-Planck -- Planck hardware support for Kaleidoscope
 * Copyright (C) 2018, 2019, 2020  Keyboard.io, Inc
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

#ifdef ARDUINO_AVR_PLANCK

#include <Arduino.h>

#include "kaleidoscope/device/ATmega32U4Keyboard.h"
#include "kaleidoscope/driver/bootloader/avr/HalfKay.h"
#include "kaleidoscope/driver/keyscanner/ATmega.h"

namespace kaleidoscope {
namespace device {
namespace olkb {

struct PlanckProps : kaleidoscope::device::ATmega32U4KeyboardProps {
  struct KeyScannerProps : public kaleidoscope::driver::keyscanner::ATmegaProps {
    static constexpr uint8_t matrix_rows    = 4;
    static constexpr uint8_t matrix_columns = 12;
    typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;
#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
    static constexpr uint8_t matrix_row_pins[matrix_rows]    = {PIN_D0, PIN_D5, PIN_B5, PIN_B6};
    static constexpr uint8_t matrix_col_pins[matrix_columns] = {PIN_F0, PIN_F1, PIN_F4, PIN_F5, PIN_F6, PIN_F7, PIN_B3, PIN_B1, PIN_B0, PIN_D5, PIN_B7, PIN_C7};
#endif  // KALEIDOSCOPE_VIRTUAL_BUILD
  };
  typedef kaleidoscope::driver::keyscanner::ATmega<KeyScannerProps> KeyScanner;
  typedef kaleidoscope::driver::bootloader::avr::HalfKay Bootloader;
  static constexpr const char *short_name = "planck";
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
class Planck : public kaleidoscope::device::ATmega32U4Keyboard<PlanckProps> {};
#else  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
/* Device definition omitted for virtual device builds.
 * We need to forward declare the device name, though, as there are
 * some legacy extern references to boards whose definition
 * depends on this.
 */
class Planck;

#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

// clang-format off
#define PER_KEY_DATA(dflt,                                                       \
         R0C0, R0C1, R0C2, R0C3, R0C4, R0C5, R0C6, R0C7, R0C8, R0C9, R0C10, R0C11, \
         R1C0, R1C1, R1C2, R1C3, R1C4, R1C5, R1C6, R1C7, R1C8, R1C9, R1C10, R1C11, \
         R2C0, R2C1, R2C2, R2C3, R2C4, R2C5, R2C6, R2C7, R2C8, R2C9, R2C10, R2C11, \
         R3C0, R3C1, R3C2, R3C3, R3C4, R3C5, R3C6, R3C7, R3C8, R3C9, R3C10, R3C11  \
  )                                                                                \
         R0C0, R0C1, R0C2, R0C3, R0C4, R0C5, R0C6, R0C7, R0C8, R0C9, R0C10, R0C11, \
         R1C0, R1C1, R1C2, R1C3, R1C4, R1C5, R1C6, R1C7, R1C8, R1C9, R1C10, R1C11, \
         R2C0, R2C1, R2C2, R2C3, R2C4, R2C5, R2C6, R2C7, R2C8, R2C9, R2C10, R2C11, \
         R3C0, R3C1, R3C2, R3C3, R3C4, R3C5, R3C6, R3C7, R3C8, R3C9, R3C10, R3C11
// clang-format on

}  // namespace olkb
}  // namespace device

EXPORT_DEVICE(kaleidoscope::device::olkb::Planck)

}  // namespace kaleidoscope

#endif

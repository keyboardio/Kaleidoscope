/* -*- mode: c++ -*-
 * Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2022  Keyboard.io, Inc
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

#ifdef ARDUINO_AVR_PLOOPY_MINI

#include <Arduino.h>

#define KALEIDOSCOPE_BOOTLOADER_FLIP_WORKAROUND 1

#include "kaleidoscope/device/ATmega32U4Keyboard.h"
#include "kaleidoscope/driver/bootloader/avr/FLIP.h"
#include "kaleidoscope/driver/keyscanner/ATmegaDirect.h"

namespace kaleidoscope {
namespace device {
namespace ploopy {

struct TrackballMiniProps : kaleidoscope::device::ATmega32U4KeyboardProps {
  struct MCUProps : public kaleidoscope::driver::mcu::ATmega32U4Props {
    static constexpr bool disable_jtag           = true;
    static constexpr bool disable_clock_division = true;
  };
  typedef kaleidoscope::driver::mcu::ATmega32U4<MCUProps> MCU;

  struct KeyScannerProps : public kaleidoscope::driver::keyscanner::ATmegaDirectProps {
    static constexpr uint8_t matrix_rows    = 1;
    static constexpr uint8_t matrix_columns = 6;
    typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
    static constexpr uint8_t direct_pins[5] = {PIN_D4, PIN_D2, PIN_E6, PIN_B6, PIN_D7};
    static constexpr uint8_t unused_pins[12] = {PIN_B5, PIN_C7, PIN_D0, PIN_D1, PIN_D3, PIN_D5, PIN_D6, PIN_F1, PIN_F3, PIN_F5, PIN_F6, PIN_F7};
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
  };

  typedef kaleidoscope::driver::keyscanner::ATmegaDirect<KeyScannerProps> KeyScanner;
  typedef kaleidoscope::driver::bootloader::avr::FLIP Bootloader;
  static constexpr const char *short_name = "ploopy_mini";
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
class TrackballMini : public kaleidoscope::device::ATmega32U4Keyboard<TrackballMiniProps> {};
#else  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
/* Device definition omitted for virtual device builds.
 * We need to forward declare the device name, though, as there are
 * some legacy extern references to boards whose definition
 * depends on this.
 */
class TrackballMini;

#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

// clang-format off

#define PER_KEY_DATA(dflt, P1, P2, P3, P4, P5, SENSOR) \
  P1, P2, P3, P4, P5, SENSOR

// clang-format on

}  // namespace keyboardio
}  // namespace device

EXPORT_DEVICE(kaleidoscope::device::ploopy::TrackballMini)

}  // namespace kaleidoscope

#endif

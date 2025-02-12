/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2019-2025 Keyboard.io, inc.
 *
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * Additional Permissions:
 * As an additional permission under Section 7 of the GNU General Public
 * License Version 3, you may link this software against a Vendor-provided
 * Hardware Specific Software Module under the terms of the MCU Vendor
 * Firmware Library Additional Permission Version 1.0.
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

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
#include <avr/wdt.h>

#ifndef KALEIDOSCOPE_BOOTLOADER_FLIP_WORKAROUND
#error To use the FLIP bootloader driver, KALEIDOSCOPE_BOOTLOADER_FLIP_WORKAROUND *must* be defined prior to including this header!
#endif
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

#include "kaleidoscope/driver/bootloader/Base.h"  // IWYU pragma: keep
#include "kaleidoscope/driver/bootloader/None.h"  // for None

namespace kaleidoscope {
namespace driver {
namespace bootloader {
namespace avr {

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
class FLIP : public kaleidoscope::driver::bootloader::Base {
 public:
  static void rebootBootloader();
};
#else   // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
typedef bootloader::None FLIP;
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

}  // namespace avr
}  // namespace bootloader
}  // namespace driver
}  // namespace kaleidoscope

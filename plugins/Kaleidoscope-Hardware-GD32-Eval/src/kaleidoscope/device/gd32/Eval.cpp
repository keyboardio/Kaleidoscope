/* -*- mode: c++ -*-
 * Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2021-2022  Keyboard.io, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef ARDUINO_GD32F303CC_GENERIC

#include "kaleidoscope/Runtime.h"

namespace kaleidoscope {
namespace device {
namespace gd32 {

void Eval::rebootBootloader() {
  USBCore().disconnect();
  NVIC_SystemReset();
}

}  // namespace gd32
}  // namespace device
}  // namespace kaleidoscope

#endif  //  ifdef ARDUINO_GD32F303CC_GENERIC

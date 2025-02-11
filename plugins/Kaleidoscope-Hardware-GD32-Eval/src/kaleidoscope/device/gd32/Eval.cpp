/* Kaleidoscope-Hardware-GD32-Eval -- GD32 Eval hardware support for Kaleidoscope
 * Copyright 2021-2025 Keyboard.io, inc.
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

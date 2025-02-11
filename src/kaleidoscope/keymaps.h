/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2025 Keyboard.io, inc.
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

#include <stdint.h>  // for uint8_t

#include "kaleidoscope/KeyAddr.h"          // for KeyAddr
#include "kaleidoscope/device/device.h"    // for Device
#include "kaleidoscope/key_defs.h"         // for Key
#include "kaleidoscope_internal/device.h"  // for device

extern const Key keymaps_linear[][kaleidoscope_internal::device.matrix_rows * kaleidoscope_internal::device.matrix_columns];

namespace kaleidoscope {

inline Key keyFromKeymap(uint8_t layer, KeyAddr key_addr) {
  return keymaps_linear[layer][key_addr.toInt()].readFromProgmem();
}

}  // namespace kaleidoscope

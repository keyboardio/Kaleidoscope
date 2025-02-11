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

namespace kaleidoscope {
namespace plugin {

class AccessTransientLEDMode {
 public:
  // This method is called when a plugin's LED mode is activated.
  // Derived plugins may reimplement it to store the id of their
  // exported LED mode. A plugin can thus check
  // whether their LED mode is currently active.
  //
  void registerLEDModeActivated(uint8_t led_mode_id) {
    led_mode_id_ = led_mode_id;
  }

 protected:
  uint8_t led_mode_id_ = 255; /* 255 means uninitialized */
};

}  // namespace plugin
}  // namespace kaleidoscope

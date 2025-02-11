/* Kaleidoscope-USB-Quirks -- USB Quirks for Kaleidoscope
 * Copyright 2018-2025 Keyboard.io, inc.
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

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/plugin.h"                // for Plugin

namespace kaleidoscope {
namespace plugin {
class USBQuirks : public kaleidoscope::Plugin {
 public:
  void toggleKeyboardProtocol();
  EventHandlerResult onSetup();
  static void setKeys(Key boot_led, Key nkro_led);

 private:
  static KeyAddr key_boot_addr;
  static KeyAddr key_nkro_addr;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::USBQuirks USBQuirks;

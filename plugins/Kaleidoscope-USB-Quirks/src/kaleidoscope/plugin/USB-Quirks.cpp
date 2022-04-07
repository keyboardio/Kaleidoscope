/* -*- mode: c++ -*-
 * Kaleidoscope-USB-Quirks -- USB Quirks for Kaleidoscope
 * Copyright (C) 2018  Keyboard.io, Inc.
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

#include "kaleidoscope/plugin/USB-Quirks.h"

#include <Arduino.h>  // for delay
#include <stdint.h>   // for uint8_t

#include "kaleidoscope/Runtime.h"                         // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"                   // for Base<>::HID, VirtualProps::HID
#include "kaleidoscope/driver/hid/keyboardio/Keyboard.h"  // for Keyboard

namespace kaleidoscope {
namespace plugin {

void USBQuirks::toggleKeyboardProtocol() {
  uint8_t new_protocol = !Runtime.hid().keyboard().getProtocol();

  Runtime.detachFromHost();
  Runtime.hid().keyboard().setDefaultProtocol(new_protocol);
  delay(1000);
  Runtime.attachToHost();
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::USBQuirks USBQuirks;

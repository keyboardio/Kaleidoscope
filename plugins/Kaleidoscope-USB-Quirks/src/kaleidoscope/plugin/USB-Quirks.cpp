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
#include "kaleidoscope/key_defs.h"                        // for Key
#include "kaleidoscope/plugin/LEDControl.h"               // for LEDControl

namespace kaleidoscope {
namespace plugin {

KeyAddr USBQuirks::key_boot_addr = KeyAddr::none();
KeyAddr USBQuirks::key_nkro_addr = KeyAddr::none();

static KeyAddr findKey(Key search_key) {
  for (auto key_addr : KeyAddr::all()) {
    Key k = Layer.lookupOnActiveLayer(key_addr);

    if (k == search_key) {
      return key_addr;
    }
  }
  return KeyAddr::none();
}

void USBQuirks::setKeys(Key boot_led, Key nkro_led) {
  key_boot_addr = findKey(boot_led);
  key_nkro_addr = findKey(nkro_led);
}

EventHandlerResult USBQuirks::onSetup() {
  setKeys(Key_B, Key_N);
  return EventHandlerResult::OK;
}

void USBQuirks::toggleKeyboardProtocol() {
  KeyAddr key_addr;
  uint8_t new_bootonly = !Runtime.hid().keyboard().getBootOnly();

  if (new_bootonly) {
    key_addr = key_boot_addr;
  } else {
    key_addr = key_nkro_addr;
  }
  ::LEDControl.disable();
  if (key_addr.isValid()) {
    ::LEDControl.setCrgbAt(key_addr, CRGB(0, 0, 255));
  }
  Runtime.device().syncLeds();
  /*
   * Release keys, because after detach, Windows 10 remembers keys that
   * were pressed (from the MagicCombo that activated this function).
   */
  Runtime.hid().keyboard().releaseAllKeys();
  Runtime.hid().keyboard().sendReport();
  delay(10);
  Runtime.detachFromHost();
  Runtime.hid().keyboard().setBootOnly(new_bootonly);
  delay(1000);
  ::LEDControl.set_all_leds_to(CRGB(0, 0, 0));
  ::LEDControl.enable();
  Runtime.attachToHost();
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::USBQuirks USBQuirks;

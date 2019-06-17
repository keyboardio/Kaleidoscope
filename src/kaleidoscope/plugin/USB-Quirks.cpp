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

#include <Kaleidoscope-USB-Quirks.h>

#if KALEIDOSCOPE_HIDADAPTOR_ENABLE_KEYBOARD_BOOT_PROTOCOL
#include "BootKeyboard/BootKeyboard.h"
#endif

namespace kaleidoscope {
namespace plugin {

void USBQuirks::toggleKeyboardProtocol() {

#if KALEIDOSCOPE_HIDADAPTOR_ENABLE_KEYBOARD_BOOT_PROTOCOL
  uint8_t new_protocol = !BootKeyboard.getProtocol();

  Kaleidoscope.detachFromHost();
  BootKeyboard.default_protocol = new_protocol;
  BootKeyboard.setProtocol(new_protocol);
  delay(1000);
  Kaleidoscope.attachToHost();
#endif

}

}
}

kaleidoscope::plugin::USBQuirks USBQuirks;

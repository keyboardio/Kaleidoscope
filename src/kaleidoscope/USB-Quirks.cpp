/* -*- mode: c++ -*-
 * Kaleidoscope-USB-Quirks -- USB Quirks for Kaleidoscope
 * Copyright (C) 2018  Gergely Nagy
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

#include <Kaleidoscope-USB-Quirks.h>

namespace kaleidoscope {

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

kaleidoscope::USBQuirks USBQuirks;

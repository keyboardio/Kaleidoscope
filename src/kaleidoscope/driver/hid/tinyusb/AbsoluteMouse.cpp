// -*- mode: c++ -*-
/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2024  Keyboard.io, Inc.
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

#ifdef USE_TINYUSB

#include "AbsoluteMouse.h"

namespace kaleidoscope {
namespace driver {
namespace hid {
namespace tinyusb {

static const uint8_t AbsoluteMouseDesc[] = {
  DESCRIPTOR_ABSOLUTE_MOUSE(),
};

TUSBAbsoluteMouse_::TUSBAbsoluteMouse_()
  : HIDD(AbsoluteMouseDesc, sizeof(AbsoluteMouseDesc), HID_ITF_PROTOCOL_NONE, 1) {}

TUSBAbsoluteMouse_ &TUSBAbsoluteMouse() {
  static TUSBAbsoluteMouse_ obj;
  return obj;
}

}  // namespace tinyusb
}  // namespace hid
}  // namespace driver
}  // namespace kaleidoscope

#endif /* USE_TINYUSB */

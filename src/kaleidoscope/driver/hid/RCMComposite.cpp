// -*- mode: c++ -*-
/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2021  Keyboard.io, Inc.
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

#include "kaleidoscope/driver/hid/RCMComposite.h"

namespace kaleidoscope {
namespace driver {
namespace hid {
namespace rcmcomposite {

USBHID RCMHID;
HIDKeyboard RCMBootKeyboard(RCMHID);
HIDConsumer RCMConsumer(RCMHID);
HIDMouse RCMMouse(RCMHID);
USBCompositeSerial CompositeSerial;

} // namespace rcmcomposite
} // namespace hid
} // namespace driver
} // namespace kaleidoscope

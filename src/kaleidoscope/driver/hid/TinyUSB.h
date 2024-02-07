// -*- mode: c++ -*-
/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2019  Keyboard.io, Inc.
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

#pragma once

#include "kaleidoscope/driver/hid/Base.h"                      // for Base, BaseProps
#include "kaleidoscope/driver/hid/tinyusb/AbsoluteMouse.h"     // for AbsoluteMouse, AbsoluteMou...

#ifdef USE_TINYUSB

namespace kaleidoscope {
namespace driver {
namespace hid {

struct TinyUSBProps : public BaseProps {
  typedef base::KeyboardProps KeyboardProps;
  typedef base::Keyboard<KeyboardProps> Keyboard;
  typedef base::MouseProps MouseProps;
  typedef base::Mouse<MouseProps> Mouse;
  typedef tinyusb::AbsoluteMouseProps AbsoluteMouseProps;
  typedef tinyusb::AbsoluteMouse<AbsoluteMouseProps> AbsoluteMouse;
};

template<typename _Props>
class TinyUSB : public Base<_Props> {};

}  // namespace hid
}  // namespace driver
}  // namespace kaleidoscope

#endif /* USE_TINY_USB */

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

#include <KeyboardioHID.h>  // for SingleAbsoluteMouse_, SingleAbso...
#include <stdint.h>         // for uint8_t, int8_t, uint16_t

#include "kaleidoscope/driver/hid/base/AbsoluteMouse.h"  // for AbsoluteMouse, AbsoluteMouseProps

// IWYU pragma: no_include "DeviceAPIs/AbsoluteMouseAPI.hpp"

namespace kaleidoscope {
namespace driver {
namespace hid {
namespace keyboardio {

/*
 * We are wrapping `SingleAbsoluteMouse` here, instead of directly using the
 * class in `AbsoluteMouseProps` below. We do this, because this lets the linker
 * optimize this whole thing out if it is unused. It can do that because
 * instantiating `SingleAbsoluteMouse` is in a separate compilation unit.
 *
 * While it would have been cleaner and shorter to instantiate them here, and
 * drop the global objects, that prevents optimizing them out, and that's a cost
 * we do not want to pay.
 */
class AbsoluteMouseWrapper {
 public:
  AbsoluteMouseWrapper() {}

  void begin() {
    SingleAbsoluteMouse.begin();
  }
  void move(int8_t x, int8_t y, int8_t wheel) {
    SingleAbsoluteMouse.move(x, y, wheel);
  }
  void moveTo(uint16_t x, uint16_t y, uint8_t wheel) {
    SingleAbsoluteMouse.moveTo(x, y, wheel);
  }

  void click(uint8_t buttons) {
    SingleAbsoluteMouse.click(buttons);
  }
  void press(uint8_t buttons) {
    SingleAbsoluteMouse.press(buttons);
  }
  void release(uint8_t buttons) {
    SingleAbsoluteMouse.release(buttons);
  }
};

struct AbsoluteMouseProps : public base::AbsoluteMouseProps {
  typedef AbsoluteMouseWrapper AbsoluteMouse;
};

template<typename _Props>
class AbsoluteMouse : public base::AbsoluteMouse<_Props> {};

}  // namespace keyboardio
}  // namespace hid
}  // namespace driver
}  // namespace kaleidoscope

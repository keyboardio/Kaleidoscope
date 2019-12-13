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
#include <Arduino.h>
#include <KeyboardioHID.h>

#include "kaleidoscope/driver/hid/base/Mouse.h"

namespace kaleidoscope {
namespace driver {
namespace hid {
namespace keyboardio {

/*
 * We are wrapping `Mouse` here, instead of directly using the class in
 * `MouseProps` below. We do this, because this lets the linker optimize this
 * whole thing out if it is unused. It can do that because instantiating `Mouse`
 * is in a separate compilation unit.
 *
 * While it would have been cleaner and shorter to instantiate them here, and
 * drop the global objects, that prevents optimizing them out, and that's a cost
 * we do not want to pay.
 */

class MouseWrapper {
 public:
  MouseWrapper() {}

  void begin() {
    Mouse.begin();
  }
  void sendReport() {
    Mouse.sendReport();
  }
  void move(int8_t x, int8_t y, int8_t vWheel, int8_t hWheel) {
    Mouse.move(x, y, vWheel, hWheel);
  }
  void releaseAll() {
    Mouse.releaseAll();
  }
  void press(uint8_t buttons) {
    Mouse.press(buttons);
  }
  void release(uint8_t buttons) {
    Mouse.release(buttons);
  }
  void click(uint8_t buttons) {
    Mouse.click(buttons);
  }
  HID_MouseReport_Data_t getReport() {
    return Mouse.getReport();
  }
};

struct MouseProps: public base::MouseProps {
  typedef MouseWrapper Mouse;
};

template <typename _Props>
class Mouse: public base::Mouse<_Props> {};

}
}
}
}

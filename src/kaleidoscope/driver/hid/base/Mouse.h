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

#include <stdint.h>  // for int8_t, uint8_t

namespace kaleidoscope {
namespace driver {
namespace hid {
namespace base {

class NoMouse {
 public:
  NoMouse() {}

  void begin() {}
  void sendReport() {}
  void move(int8_t x, int8_t y, int8_t vWheel, int8_t hWheel) {}
  void stop(bool x, bool y, bool vWheel, bool hWheel) {}
  void releaseAll() {}
  void press(uint8_t buttons) {}
  void release(uint8_t buttons) {}
  void click(uint8_t buttons) {}
};

struct MouseProps {
  typedef NoMouse Mouse;
};

template<typename _Props>
class Mouse {
 private:
  typename _Props::Mouse mouse_;

 public:
  Mouse() {}

  void setup() {
    mouse_.begin();
  }

  void sendReport() {
    mouse_.sendReport();
  }

  void move(int8_t x, int8_t y, int8_t vWheel = 0, int8_t hWheel = 0) {
    mouse_.move(x, y, vWheel, hWheel);
  }
  void stop(bool x, bool y, bool vWheel = false, bool hWheel = false) {
    mouse_.stop(x, y, vWheel, hWheel);
  }
  void releaseAllButtons() {
    mouse_.releaseAll();
  }
  void pressButtons(uint8_t buttons) {
    mouse_.press(buttons);
  }
  void releaseButtons(uint8_t buttons) {
    mouse_.release(buttons);
  }
  void clickButtons(uint8_t buttons) {
    mouse_.click(buttons);
  }
};

}  // namespace base
}  // namespace hid
}  // namespace driver
}  // namespace kaleidoscope

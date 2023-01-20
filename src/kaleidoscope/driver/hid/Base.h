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

#include "base/AbsoluteMouse.h"  // for AbsoluteMouse, AbsoluteMouseProps
#include "base/Keyboard.h"       // for Keyboard, KeyboardProps
#include "base/Mouse.h"          // for Mouse, MouseProps

namespace kaleidoscope {
namespace driver {
namespace hid {

struct BaseProps {
  typedef base::KeyboardProps KeyboardProps;
  typedef base::Keyboard<KeyboardProps> Keyboard;
  typedef base::MouseProps MouseProps;
  typedef base::Mouse<MouseProps> Mouse;
  typedef base::AbsoluteMouseProps AbsoluteMouseProps;
  typedef base::AbsoluteMouse<AbsoluteMouseProps> AbsoluteMouse;
};

template<typename _Props>
class Base {
 private:
  typename _Props::Keyboard keyboard_;
  typename _Props::Mouse mouse_;
  typename _Props::AbsoluteMouse absolute_mouse_;

 public:
  Base() {}

  void setup() {
    keyboard().setup();
  }

  void onUSBReset() {
    keyboard().onUSBReset();
  }

  auto keyboard() -> decltype(keyboard_) & {
    return keyboard_;
  }

  auto mouse() -> decltype(mouse_) & {
    return mouse_;
  }

  auto absoluteMouse() -> decltype(absolute_mouse_) & {
    return absolute_mouse_;
  }
};

}  // namespace hid
}  // namespace driver
}  // namespace kaleidoscope

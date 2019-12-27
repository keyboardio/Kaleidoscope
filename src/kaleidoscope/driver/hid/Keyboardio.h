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
#include "kaleidoscope/key_defs.h"

#include "kaleidoscope/driver/hid/Base.h"

#include "keyboardio/Keyboard.h"
#include "keyboardio/Mouse.h"
#include "keyboardio/AbsoluteMouse.h"

namespace kaleidoscope {
namespace driver {
namespace hid {

struct KeyboardioProps: public BaseProps {
  typedef keyboardio::KeyboardProps KeyboardProps;
  typedef keyboardio::Keyboard<KeyboardProps> Keyboard;
  typedef keyboardio::MouseProps MouseProps;
  typedef keyboardio::Mouse<MouseProps> Mouse;
  typedef keyboardio::AbsoluteMouseProps AbsoluteMouseProps;
  typedef keyboardio::AbsoluteMouse<AbsoluteMouseProps> AbsoluteMouse;
};

template <typename _Props>
class Keyboardio: public Base<_Props> {};

}
}
}

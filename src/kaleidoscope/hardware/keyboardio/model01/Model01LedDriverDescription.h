/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Model01 -- Keyboard.io Model01 hardware support for Kaleidoscope
 * Copyright (C) 2017-2019  Keyboard.io, Inc
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

#ifdef ARDUINO_AVR_MODEL01

#include <Arduino.h>

#include "kaleidoscope/driver/BaseLedDriverDescription.h"

namespace kaleidoscope {
namespace hardware {
namespace keyboardio {

struct Model01LedDriverDescription : public kaleidoscope::driver::BaseLedDriverDescription {
  static constexpr LedCountType led_count = 64;
};

}
}
}

#endif

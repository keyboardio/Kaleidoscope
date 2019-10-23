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

#include "kaleidoscope/driver/BaseLedDriver.h"
#include "kaleidoscope/hardware/keyboardio/model01/Model01LedDriverDescription.h"

namespace kaleidoscope {
namespace hardware {
namespace keyboardio {

class Model01LedDriver : public kaleidoscope::driver::BaseLedDriver<Model01LedDriverDescription> {
 public:
  Model01LedDriver() {}

  static void setup();
  static void syncLeds();
  static void setCrgbAt(int8_t i, cRGB crgb);
  static cRGB getCrgbAt(int8_t i);

  static int8_t getLedIndex(KeyAddr key_addr);

  static void enableHighPowerLeds();
  static boolean ledPowerFault();

 private:
  static bool isLEDChanged;
};

}
}
}

#endif

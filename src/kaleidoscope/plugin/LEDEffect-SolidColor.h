/* Kaleidoscope-LEDEffect-SolidColor - Solid color LED effects for Kaleidoscope.
 * Copyright (C) 2017-2019  Keyboard.io, Inc.
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

#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-UnderglowControl.h"

namespace kaleidoscope {
namespace plugin {

class LEDSolidColor : public LEDMode {
 public:
  LEDSolidColor(uint8_t r, uint8_t g, uint8_t b);

 protected:
  void onActivate(void) final;
  void refreshAt(byte row, byte col) final;

 private:
  uint8_t r, g, b;
};

class UnderglowSolidColor : public UnderglowEffect {
 public:
  UnderglowSolidColor(uint8_t r, uint8_t g, uint8_t b)
    : r_(r), g_(g), b_(b) {}

 protected:
  void onActivate(void) final;
  void refreshAt(uint8_t index) final;

 private:
  uint8_t r_, g_, b_;
};

}
}

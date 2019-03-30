/* Kaleidoscope-LEDEffect-SolidColor - Solid color LED effects for Kaleidoscope.
 * Copyright (C) 2017  Keyboard.io, Inc.
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

#include "Kaleidoscope-LEDEffect-SolidColor.h"

namespace kaleidoscope {
namespace plugin {
LEDSolidColor::LEDSolidColor(uint8_t r, uint8_t g, uint8_t b) {
  this->r = r;
  this->g = g;
  this->b = b;
}

void LEDSolidColor::onActivate(void) {
  ::LEDControl.set_all_leds_to(r, g, b);
}

void LEDSolidColor::refreshAt(KeyLEDAddr led_addr) {
  ::LEDControl.setCrgbAt(led_addr, CRGB(r, g, b));
}

}
}

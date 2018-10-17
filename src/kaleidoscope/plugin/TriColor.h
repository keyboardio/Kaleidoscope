/* -*- mode: c++ -*-
 * Kaleidoscope-LEDEffects -- An assorted collection of LED effects for Kaleidoscope
 * Copyright (C) 2016, 2017  Keyboard.io, Inc
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

#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>

namespace kaleidoscope {
namespace plugin {
class TriColor : public LEDMode {
 public:
  TriColor(cRGB base_color, cRGB mod_color, cRGB esc_color);
  TriColor(cRGB base_color, cRGB mod_color) : TriColor(base_color, mod_color, mod_color) {}

 protected:
  void update(void) final;

 private:
  cRGB base_color_;
  cRGB mod_color_;
  cRGB esc_color_;
};
}
}

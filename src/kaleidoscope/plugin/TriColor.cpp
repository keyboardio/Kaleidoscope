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

#include <Kaleidoscope-LEDEffects.h>

namespace kaleidoscope {
namespace plugin {

TriColor::TriColor(cRGB base_color, cRGB mod_color, cRGB esc_color) {
  base_color_ = base_color;
  mod_color_ = mod_color;
  esc_color_ = esc_color;
}

void TriColor::update(void) {
   for(auto key_addr: KeyAddr{}) {
      Key k = Layer.lookup(key_addr);

      // Special keys are always mod_color
      if (k.flags != 0) {
        ::LEDControl.setCrgbAt(KeyLEDAddr(key_addr), mod_color_);
        continue;
      }

      cRGB color = mod_color_;

      switch (k.keyCode) {
      case Key_A.keyCode ... Key_0.keyCode:
      case Key_Spacebar.keyCode:
      case Key_KeypadDivide.keyCode ... Key_KeypadSubtract.keyCode:
      case Key_Keypad1.keyCode ... Key_KeypadDot.keyCode:
      case Key_F1.keyCode ... Key_F4.keyCode:
      case Key_F9.keyCode ... Key_F12.keyCode:
        color = base_color_;
        break;
      case Key_Escape.keyCode:
        color = esc_color_;
        break;
      }

      ::LEDControl.setCrgbAt(KeyLEDAddr(key_addr), color);
    }
}

}
}

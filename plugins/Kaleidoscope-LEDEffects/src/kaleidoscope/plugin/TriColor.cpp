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

#include "kaleidoscope/plugin/TriColor.h"

#include "kaleidoscope/KeyAddr.h"            // for KeyAddr, MatrixAddr, MatrixAddr<>::Range
#include "kaleidoscope/device/device.h"      // for cRGB
#include "kaleidoscope/key_defs.h"           // for Key, Key_0, Key_A, Key_Escape, Key_F1, Key_F12
#include "kaleidoscope/layers.h"             // for Layer, Layer_
#include "kaleidoscope/plugin/LEDControl.h"  // for LEDControl

namespace kaleidoscope {
namespace plugin {

TriColor::TriColor(cRGB base_color, cRGB mod_color, cRGB esc_color) {
  base_color_ = base_color;
  mod_color_  = mod_color;
  esc_color_  = esc_color;
}

void TriColor::TransientLEDMode::update() {
  for (auto key_addr : KeyAddr::all()) {
    Key k = Layer.lookupOnActiveLayer(key_addr);

    // Special keys are always mod_color
    if (k.getFlags() != 0) {
      ::LEDControl.setCrgbAt(KeyAddr(key_addr), parent_->mod_color_);
      continue;
    }

    cRGB color = parent_->mod_color_;

    switch (k.getKeyCode()) {
    case Key_A.getKeyCode()... Key_0.getKeyCode():
    case Key_Spacebar.getKeyCode():
    case Key_KeypadDivide.getKeyCode()... Key_KeypadSubtract.getKeyCode():
    case Key_Keypad1.getKeyCode()... Key_KeypadDot.getKeyCode():
    case Key_F1.getKeyCode()... Key_F4.getKeyCode():
    case Key_F9.getKeyCode()... Key_F12.getKeyCode():
      color = parent_->base_color_;
      break;
    case Key_Escape.getKeyCode():
      color = parent_->esc_color_;
      break;
    }

    ::LEDControl.setCrgbAt(KeyAddr(key_addr), color);
  }
}

}  // namespace plugin
}  // namespace kaleidoscope

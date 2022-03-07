/* -*- mode: c++ -*-
 * Kaleidoscope-LED-ActiveModColor -- Light up the LEDs under the active modifiers
 * Copyright (C) 2016-2020  Keyboard.io, Inc
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

#include "kaleidoscope/Runtime.h"
#include "kaleidoscope/KeyAddrBitfield.h"
#include <Kaleidoscope-LEDControl.h>

#define MAX_MODS_PER_LAYER 16

namespace kaleidoscope {
namespace plugin {
class ActiveModColorEffect : public kaleidoscope::Plugin {
 public:
  ActiveModColorEffect(void) {}

  static void setHighlightColor(cRGB color) {
    highlight_color_ = color;
  }
  static void setOneShotColor(cRGB color) {
    oneshot_color_ = color;
  }
  static void setStickyColor(cRGB color) {
    sticky_color_ = color;
  }

  static void highlightNormalModifiers(bool value) {
    highlight_normal_modifiers_ = value;
  }

  EventHandlerResult onKeyEvent(KeyEvent &event);
  EventHandlerResult beforeSyncingLeds();

 private:
  static bool highlight_normal_modifiers_;
  static KeyAddrBitfield mod_key_bits_;

  static cRGB highlight_color_;
  static cRGB oneshot_color_;
  static cRGB sticky_color_;
};
}
}

extern kaleidoscope::plugin::ActiveModColorEffect ActiveModColorEffect;

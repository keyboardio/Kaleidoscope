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

// =============================================================================
#define _DEPRECATED_MESSAGE_ACTIVEMODCOLOR_COLORS                              \
  "The `ActiveModColorEffect` public class variables have been deprecated. \n" \
  "Please use the following methods instead:                               \n" \
  " - for `highlight_color` => `setHighlightColor(color)`                  \n" \
  " - for `oneshot_color` => `setOneShotColor(color)`                      \n" \
  " - for `sticky_color` => `setStickyColor(color)`                        \n" \
  "These variables will be removed after 2021-08-01."

namespace kaleidoscope {
namespace plugin {
class ActiveModColorEffect : public kaleidoscope::Plugin {
 public:
  ActiveModColorEffect(void) {}

  // When removing access to these variables, don't delete them. Instead, make
  // them private, and add trailing underscores here and in
  // LED-ActiveModColor.cpp.
  DEPRECATED(ACTIVEMODCOLOR_COLORS)
  static cRGB highlight_color;
  DEPRECATED(ACTIVEMODCOLOR_COLORS)
  static cRGB oneshot_color;
  DEPRECATED(ACTIVEMODCOLOR_COLORS)
  static cRGB sticky_color;

  static void setHighlightColor(cRGB color) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    highlight_color = color;
#pragma GCC diagnostic pop
  }
  static void setOneShotColor(cRGB color) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    oneshot_color = color;
#pragma GCC diagnostic pop
  }
  static void setOnestickyColor(cRGB color) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    sticky_color = color;
#pragma GCC diagnostic pop
  }

  static void highlightNormalModifiers(bool value) {
    highlight_normal_modifiers_ = value;
  }

  EventHandlerResult onKeyEvent(KeyEvent &event);
  EventHandlerResult beforeSyncingLeds();

 private:
  static bool highlight_normal_modifiers_;
  static KeyAddrBitfield mod_key_bits_;
};
}
}

extern kaleidoscope::plugin::ActiveModColorEffect ActiveModColorEffect;

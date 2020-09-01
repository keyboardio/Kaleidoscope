/* -*- mode: c++ -*-
 * Kaleidoscope-LED-ActiveModColor -- Light up the LEDs under the active modifiers
 * Copyright (C) 2016, 2017, 2018  Keyboard.io, Inc
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

  static cRGB highlight_color;
  static cRGB sticky_color;

  static void highlightNormalModifiers(bool value) {
    highlight_normal_modifiers_ = value;
  }

  EventHandlerResult onKeyswitchEvent(Key &key,
                                      KeyAddr key_addr,
                                      uint8_t key_state);
  EventHandlerResult beforeReportingState();

 private:
  static bool highlight_normal_modifiers_;
  static KeyAddrBitfield mod_key_bits_;
};
}
}

extern kaleidoscope::plugin::ActiveModColorEffect ActiveModColorEffect;

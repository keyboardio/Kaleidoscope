/* -*- mode: c++ -*-
 * Kaleidoscope-TopsyTurvy -- Turn the effect of Shift upside down for certain keys
 * Copyright (C) 2017, 2018  Keyboard.io, Inc
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
#include <Kaleidoscope-Ranges.h>

#define TOPSY(k) Key(kaleidoscope::ranges::TT_FIRST + (Key_ ## k).getKeyCode())

namespace kaleidoscope {
namespace plugin {

class TopsyTurvy: public kaleidoscope::Plugin {
 public:
  TopsyTurvy(void) {}

  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state);

 private:
  static uint8_t last_pressed_position_;
  static bool is_shifted_;
  static bool is_active_;
};
}
}

extern kaleidoscope::plugin::TopsyTurvy TopsyTurvy;

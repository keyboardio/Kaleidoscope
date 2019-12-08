/* -*- mode: c++ -*-
 * Kaleidoscope-Redial -- Redial support for Kaleidoscope
 * Copyright (C) 2018, 2019  Keyboard.io, Inc.
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

#define Key_Redial Key(kaleidoscope::ranges::REDIAL)

namespace kaleidoscope {
namespace plugin {

class Redial : public kaleidoscope::Plugin {
 public:
  Redial(void) {}

  static bool shouldRemember(Key mappedKey);

  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state);

 private:
  static Key key_to_redial_;
  static Key last_key_;
  static bool redial_held_;
};

}
}

extern kaleidoscope::plugin::Redial Redial;

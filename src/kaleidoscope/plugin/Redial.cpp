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

#include <Kaleidoscope-Redial.h>
#include "kaleidoscope/keyswitch_state.h"

namespace kaleidoscope {
namespace plugin {

Key Redial::key_to_redial_;
Key Redial::last_key_;
bool Redial::redial_held_ = false;

EventHandlerResult Redial::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state) {
  if (mapped_key == Key_Redial) {
    if (keyToggledOff(key_state))
      key_to_redial_ = last_key_;

    mapped_key = key_to_redial_;
    redial_held_ = keyIsPressed(key_state);

    return EventHandlerResult::OK;
  }

  if (keyToggledOn(key_state) && shouldRemember(mapped_key)) {
    last_key_ = mapped_key;
    if (!redial_held_)
      key_to_redial_ = mapped_key;
  }

  return EventHandlerResult::OK;
}

__attribute__((weak)) bool Redial::shouldRemember(Key mapped_key) {
  if (mapped_key >= Key_A && mapped_key <= Key_Z)
    return true;
  if (mapped_key >= Key_1 && mapped_key <= Key_0)
    return true;

  return false;
}

}
}

kaleidoscope::plugin::Redial Redial;

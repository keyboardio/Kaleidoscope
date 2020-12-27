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

#include <Kaleidoscope-TopsyTurvy.h>
#include "kaleidoscope/keyswitch_state.h"

namespace kaleidoscope {
namespace plugin {

uint8_t TopsyTurvy::last_pressed_position_;
bool TopsyTurvy::is_shifted_;
bool TopsyTurvy::is_active_;

EventHandlerResult TopsyTurvy::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state) {

  if (mapped_key == Key_LeftShift ||
      mapped_key == Key_RightShift) {
    is_shifted_ = keyIsPressed(key_state);
    if (is_active_)
      return EventHandlerResult::EVENT_CONSUMED;
  }

  if (mapped_key < ranges::TT_FIRST || mapped_key > ranges::TT_LAST) {
    if (keyToggledOn(key_state) && (mapped_key < Key_LeftControl || mapped_key > Key_RightGui)) {
      last_pressed_position_ = key_addr.toInt();
    }

    return EventHandlerResult::OK;
  }

  is_active_ = keyIsPressed(key_state);

  if (keyToggledOn(key_state)) {
    last_pressed_position_ = key_addr.toInt();
  } else {
    if (last_pressed_position_ != key_addr.toInt()) {
      return EventHandlerResult::EVENT_CONSUMED;
    }
  }

  mapped_key.setRaw(mapped_key.getRaw() - ranges::TT_FIRST);

  // invert the shift state
  if (!is_shifted_) {
    mapped_key.setFlags(mapped_key.getFlags() | SHIFT_HELD);
    return EventHandlerResult::OK;
  }

  if (keyIsPressed(key_state)) {
    kaleidoscope::Runtime.hid().keyboard().releaseKey(Key_LeftShift);
    kaleidoscope::Runtime.hid().keyboard().releaseKey(Key_RightShift);

    return EventHandlerResult::OK;
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

}
}

kaleidoscope::plugin::TopsyTurvy TopsyTurvy;

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
#include "kaleidoscope/hid.h"

namespace kaleidoscope {

uint8_t TopsyTurvy::mod_state_;
uint8_t TopsyTurvy::last_pressed_position_;
bool TopsyTurvy::is_active_;

EventHandlerResult TopsyTurvy::onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t key_state) {
  if (mapped_key.raw == Key_LeftShift.raw) {
    bitWrite(mod_state_, 0, keyIsPressed(key_state));
    if (is_active_)
      return EventHandlerResult::EVENT_CONSUMED;
  }
  if (mapped_key.raw == Key_RightShift.raw) {
    bitWrite(mod_state_, 1, keyIsPressed(key_state));
    if (is_active_)
      return EventHandlerResult::EVENT_CONSUMED;
  }

  if (mapped_key < ranges::TT_FIRST || mapped_key > ranges::TT_LAST) {
    if (keyToggledOn(key_state) && (mapped_key < Key_LeftControl || mapped_key > Key_RightGui)) {
      last_pressed_position_ = row * COLS + col;
    }

    return EventHandlerResult::OK;
  }

  if (keyToggledOn(key_state)) {
    last_pressed_position_ = row * COLS + col;
  } else {
    if (last_pressed_position_ != row * COLS + col) {
      return EventHandlerResult::EVENT_CONSUMED;
    }
  }

  is_active_ = keyIsPressed(key_state);

  // invert the shift state
  if (!mod_state_) {
    mapped_key.raw = mapped_key.raw - ranges::TT_FIRST;
    mapped_key.flags |= SHIFT_HELD;
    return EventHandlerResult::OK;
  }

  if (keyIsPressed(key_state)) {
    hid::releaseKey(Key_LeftShift);
    hid::releaseKey(Key_RightShift);

    mapped_key.raw = mapped_key.raw - ranges::TT_FIRST;
    return EventHandlerResult::OK;
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

}

kaleidoscope::TopsyTurvy TopsyTurvy;

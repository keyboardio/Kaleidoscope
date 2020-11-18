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

KeyAddr TopsyTurvy::tt_addr_ = KeyAddr::none();
bool TopsyTurvy::shift_detected_ = false;

EventHandlerResult TopsyTurvy::beforeEachCycle() {
  // Clear the shift detection state before each scan cycle.
  shift_detected_ = false;
  return EventHandlerResult::OK;
}

EventHandlerResult TopsyTurvy::beforeReportingState() {
  // If no TopsyTurvy key is active, there's nothing to do.
  if (! tt_addr_.isValid())
    return EventHandlerResult::OK;

  // A TopsyTurvy key is active. That means we need to reverse the shift state,
  // whether it was on or off.
  if (shift_detected_) {
    kaleidoscope::Runtime.hid().keyboard().releaseKey(Key_LeftShift);
    kaleidoscope::Runtime.hid().keyboard().releaseKey(Key_RightShift);
  } else {
    kaleidoscope::Runtime.hid().keyboard().pressKey(Key_LeftShift);
  }
  return EventHandlerResult::OK;
}

EventHandlerResult TopsyTurvy::onKeyswitchEvent(Key &key,
                                                KeyAddr key_addr,
                                                uint8_t key_state) {
  // If a modifer key (including combo modifiers, but not non-modifier keys with
  // mod flags) is active, and it includes `shift` (either from its keycode or a
  // mod flag), record that we detected an "intentional shift".
  if (key.isKeyboardShift() && keyIsPressed(key_state))
    shift_detected_ = true;

  // If the active TopsyTurvy key toggles off, clear the stored address to
  // record that.
  if (keyToggledOff(key_state)) {
    if (key_addr == tt_addr_) {
      tt_addr_.clear();
    }
    return EventHandlerResult::OK;
  }

  if (keyToggledOn(key_state)) {
    if (isTopsyTurvyKey(key)) {
      // If a TopsyTurvy key toggles on, store its address to indicate that it's
      // active, and decode its key value to store in the active keys cache.
      tt_addr_ = key_addr;
      key = Key(key.getRaw() - ranges::TT_FIRST);
    } else {
      // If any other key toggles on, clear the active TopsyTurvy address.
      tt_addr_.clear();
    }
  }
  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::TopsyTurvy TopsyTurvy;

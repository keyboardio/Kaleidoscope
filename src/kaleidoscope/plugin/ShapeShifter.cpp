/* -*- mode: c++ -*-
 * Kaleidoscope-ShapeShifter -- Change the shifted symbols on any key of your choice
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

#include <Kaleidoscope-ShapeShifter.h>

namespace kaleidoscope {
namespace plugin {

const ShapeShifter::dictionary_t *ShapeShifter::dictionary = NULL;
bool ShapeShifter::mod_active_;

EventHandlerResult ShapeShifter::beforeReportingState() {
  mod_active_ = kaleidoscope::Runtime.hid().keyboard().isModifierKeyActive(Key_LeftShift) ||
                kaleidoscope::Runtime.hid().keyboard().isModifierKeyActive(Key_RightShift);
  return EventHandlerResult::OK;
}

EventHandlerResult ShapeShifter::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state) {
  if (!dictionary)
    return EventHandlerResult::OK;

  // If Shift is not active, bail out early.
  if (!mod_active_)
    return EventHandlerResult::OK;

  Key orig, repl;

  // Try to find the current key in the dictionary
  uint8_t i = 0;
  do {
    orig = dictionary[i].original.readFromProgmem();
    i++;
  } while (orig != Key_NoKey &&
           orig != mapped_key);
  i--;

  // If not found, bail out.
  if (orig == Key_NoKey)
    return EventHandlerResult::OK;

  repl = dictionary[i].replacement.readFromProgmem();

  // If found, handle the alternate key instead
  mapped_key = repl;
  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::ShapeShifter ShapeShifter;

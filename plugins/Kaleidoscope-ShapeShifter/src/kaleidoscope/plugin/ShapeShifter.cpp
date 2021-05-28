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
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/LiveKeys.h"

namespace kaleidoscope {
namespace plugin {

const ShapeShifter::dictionary_t *ShapeShifter::dictionary = nullptr;

EventHandlerResult ShapeShifter::onKeyEvent(KeyEvent &event) {
  if (dictionary == nullptr)
    return EventHandlerResult::OK;

  if (!dictionary)
    return EventHandlerResult::OK;

  Key orig, repl;

  // Try to find the current key in the dictionary
  uint8_t i = 0;
  do {
    orig = dictionary[i].original.readFromProgmem();
    i++;
  } while (orig != Key_NoKey &&
           orig != event.key);
  i--;

  // If not found, bail out.
  if (orig == Key_NoKey)
    return EventHandlerResult::OK;

  bool shift_detected = false;

  for (KeyAddr k : KeyAddr::all()) {
    if (live_keys[k].isKeyboardShift())
      shift_detected = true;
  }
  if (! shift_detected)
    return EventHandlerResult::OK;

  repl = dictionary[i].replacement.readFromProgmem();

  // If found, handle the alternate key instead
  event.key = repl;
  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::ShapeShifter ShapeShifter;

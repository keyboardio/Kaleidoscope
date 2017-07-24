/* -*- mode: c++ -*-
 * Kaleidoscope-SpaceCadet -- Space Cadet Shift
 * Copyright (C) 2016, 2017  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Kaleidoscope-SpaceCadet.h>

namespace kaleidoscope {

uint8_t SpaceCadetShift::paren_needed_;
uint32_t SpaceCadetShift::start_time_;
uint16_t SpaceCadetShift::time_out = 1000;
Key SpaceCadetShift::opening_paren = Key_9, SpaceCadetShift::closing_paren = Key_0;

SpaceCadetShift::SpaceCadetShift() {
}

void SpaceCadetShift::begin() {
  event_handler_hook_use(eventHandlerHook);
}

Key SpaceCadetShift::eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state) {
  // If nothing happened, bail out fast.
  if (!keyIsPressed(key_state) && !keyWasPressed(key_state)) {
    return mapped_key;
  }

  // If a key has been just toggled on...
  if (keyToggledOn(key_state)) {
    if (mapped_key.raw == Key_LeftShift.raw) {  // if it is LShift, remember it
      bitWrite(paren_needed_, 0, 1);
      start_time_ = millis();
    } else if (mapped_key.raw == Key_RightShift.raw) {  // if it is RShift, remember it
      bitWrite(paren_needed_, 1, 1);
      start_time_ = millis();
    } else {  // if it is something else, we do not need a paren at the end.
      paren_needed_ = 0;
      start_time_ = 0;
    }

    // this is all we need to do on keypress, let the next handler do its thing too.
    return mapped_key;
  }

  // if the state is empty, that means that either the shifts weren't pressed,
  // or we used another key in the interim. in both cases, nothing special to do.
  if (!paren_needed_)
    return mapped_key;

  // if we timed out, that means we need to keep pressing shift, but won't
  // need the parens in the end.
  if ((millis() - start_time_) >= time_out) {
    paren_needed_ = 0;
    return mapped_key;
  }

  // if we have a state, but the key in question is not either of the shifts,
  // return. This can happen when another key is released, and that should not
  // interrupt us.
  if (mapped_key.raw != Key_LeftShift.raw &&
      mapped_key.raw != Key_RightShift.raw)
    return mapped_key;

  // if a key toggled off (and that must be one of the shifts at this point),
  // send the parens too (if we were interrupted, we bailed out earlier).
  if (keyToggledOff(key_state)) {
    Key paren = opening_paren;
    if (bitRead(paren_needed_, 1))
      paren = closing_paren;

    handle_keyswitch_event(mapped_key, row, col, IS_PRESSED | INJECTED);
    handle_keyswitch_event(paren, row, col, IS_PRESSED | INJECTED);
    Keyboard.sendReport();

    paren_needed_ = 0;
  }

  return mapped_key;
}

}

kaleidoscope::SpaceCadetShift SpaceCadetShift;

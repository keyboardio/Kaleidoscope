/* -*- mode: c++ -*-
 * Kaleidoscope-SpaceCadet -- Space Cadet Shift Extended
 * Copyright (C) 2016, 2017  Gergely Nagy, Ben Gemperline
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
#include <kaleidoscope/hid.h>

namespace kaleidoscope {


//Constructor with input and output, and assume default timeout
SpaceCadet::KeyBinding::KeyBinding(Key input_, Key output_) {
  input = input_;
  output = output_;
}

//Constructor with all three set
SpaceCadet::KeyBinding::KeyBinding(Key input_, Key output_, uint16_t timeout_) {
  input = input_;
  output = output_;
  timeout = timeout_;
}

//Space Cadet
KeyBinding * SpaceCadet::map = {
  //By default, respect the default timeout
  {Key_LeftShift, Key_LeftParen, 0}
  , {Key_RightShift, Key_RightParen, 0}
  //These may be uncommented, added, or set in the main sketch
  /*,{Key_LeftGui,Key_LeftCurlyBracket, 250}
  ,{Key_RightAlt,Key_RightCurlyBracket, 250}
  ,{Key_LeftControl,Key_LeftBracket, 250}
  ,{Key_RightControl,Key_RightBracket, 250}*/
  , SPACECADET_MAP_END
};
uint16_t SpaceCadet::time_out = 1000;

void SpaceCadet::begin() {
  Kaleidoscope.useEventHandlerHook(eventHandlerHook);
}

Key SpaceCadet::eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state) {

  // If nothing happened, bail out fast.
  if (!keyIsPressed(key_state) && !keyWasPressed(key_state)) {
    return mapped_key;
  }

  // If a key has been just toggled on...
  if (keyToggledOn(key_state)) {
    
    //This will only set one key, and if it isn't in our map it clears everything
    //for the non-pressed key
    for (uint8_t i = 0 ;; ++i) {
      //Check for ending sentinal and exit
      if(
        map[i].input.raw == Key_NoKey.raw
        && map[i].output.raw == Key_NoKey.raw
        && map[i].timeout == 0
      ) {
        break;
      }

      if (mapped_key.raw == map[i].input.raw) {
        //The keypress was valid and a match.
        map[i].flagged = true;
        map[i].start_time = millis();
      } else {
        //The keypress wasn't a match.
        map[i].flagged = false;
        map[i].start_time = 0;
      }
    }

    // this is all we need to do on keypress, let the next handler do its thing too.
    return mapped_key;
  }

  // if the state is empty, that means that either the shifts weren't pressed,
  // or we used another key in the interim. in both cases, nothing special to do.
  bool valid_key = false;
  bool pressed_key_was_valid = false;
  uint8_t index = 0;

  //Look to see if any keys in our map  are flagged
  for (uint8_t i = 0 ;; ++i) {

    //Check for ending sentinal and exit
    if(
      map[i].input.raw == Key_NoKey.raw
      && map[i].output.raw == Key_NoKey.raw
      && map[i].timeout == 0
    ) {
      break;
    }

    if (map[i].flagged) {
      valid_key = true;
      index = i;
    }
    if (map[i].input.raw == mapped_key.raw) {
      pressed_key_was_valid = true;
    }
  }


  //If no valid mapped keys were pressed, simply return the keycode
  if (!valid_key) {
    return mapped_key;
  }

  //use the map index to find the local timeout for this key
  uint16_t current_timeout = map[index].timeout;
  //If that isn't set, use the global timeout setting.
  if (current_timeout == 0) {
    current_timeout = time_out;
  }

  if ((millis() - map[index].start_time) >= current_timeout) {
    // if we timed out, that means we need to keep pressing the mapped
    // key, but we won't need to send the alternative key in the end
    map[index].flagged = false;
    map[index].start_time = 0;
    return mapped_key;
  }

  // If the key that was pressed isn't one of our mapped keys, just
  // return. This can happen when another key is released, and that should not
  // interrupt us.

  if (!pressed_key_was_valid) {
    return mapped_key;
  }

  // if a key toggled off (and that must be one of the mapped keys at this point),
  // send the alternative key instead (if we were interrupted, we bailed out earlier).
  if (keyToggledOff(key_state)) {
    Key alternate_key = map[index].output;

    //Since we are sending the actual key (no need for shift, etc),
    //only need to send that key and not the original key. In fact, we
    //may want to even UNSET the originally pressed key (future
    //enhanacement?).  This might also mean we don't need to return the
    //key that was pressed, though I haven't confirmed that.
    handleKeyswitchEvent(alternate_key, row, col, IS_PRESSED | INJECTED);
    hid::sendKeyboardReport();

    //Unflag the key so we don't try this again.
    map[index].flagged = false;
    map[index].start_time = 0;
  }

  return mapped_key;
}

}

kaleidoscope::SpaceCadet SpaceCadet;

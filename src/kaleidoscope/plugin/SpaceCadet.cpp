/* -*- mode: c++ -*-
 * Kaleidoscope-SpaceCadet -- Space Cadet Shift Extended
 * Copyright (C) 2016, 2017, 2018  Keyboard.io, Inc, Ben Gemperline
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

#include <Kaleidoscope-SpaceCadet.h>
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/key_events.h"

namespace kaleidoscope {
namespace plugin {

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
SpaceCadet::KeyBinding * SpaceCadet::map;
uint16_t SpaceCadet::time_out = 1000;
bool SpaceCadet::disabled = false;

SpaceCadet::SpaceCadet() {
  static SpaceCadet::KeyBinding initialmap[] = {
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

  map = initialmap;
}

//Function to enable SpaceCadet behavior
void SpaceCadet::enable() {
  disabled = false;
}

//Function to disable SpaceCadet behavior
void SpaceCadet::disable() {
  disabled = true;
}

//Function to determine whether SpaceCadet is active (useful for Macros and other plugins)
bool SpaceCadet::active() {
  return !disabled;
}

EventHandlerResult SpaceCadet::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state) {
  //Handle our synthetic keys for enabling and disabling functionality
  if (mapped_key.getRaw() >= kaleidoscope::ranges::SC_FIRST &&
      mapped_key.getRaw() <= kaleidoscope::ranges::SC_LAST) {
    //Only fire the activate / deactivate on the initial press (not held or release)
    if (keyToggledOn(key_state)) {
      if (mapped_key == Key_SpaceCadetEnable) {
        enable();
      } else if (mapped_key == Key_SpaceCadetDisable) {
        disable();
      }
    }

    return EventHandlerResult::EVENT_CONSUMED;
  }

  //if SpaceCadet is disabled, this was an injected key, it was NoKey,
  //or if they key somehow came here without being either pressed or released,
  //return the mapped key and just get out of here.
  if (
    disabled
    || (key_state & INJECTED)
    || mapped_key == Key_NoKey
    || (!keyIsPressed(key_state) && !keyWasPressed(key_state))
  ) {
    return EventHandlerResult::OK;
  }

  // If a key has been just toggled on...
  if (keyToggledOn(key_state)) {

    //check to see if we found a valid key. Assume not valid.
    bool valid_key = false;
    bool other_mapped_key_flagged = false;

    //Check the current map to see if any other key has been already flagged
    //Exit condition is if we reach the special SPACECADET_MAP_END sentinel
    for (
      uint8_t i = 0 ;
      !(
        map[i].input == Key_NoKey
        && map[i].output == Key_NoKey
        && map[i].timeout == 0
      ) ;
      ++i
    ) {

      if (map[i].flagged
          && map[i].input != mapped_key) {
        other_mapped_key_flagged = true;
        break;
      }
    }

    //This will only set one key, and, if it isn't in our map, it clears everything for the non-pressed key
    //Exit condition is if we reach the special SPACECADET_MAP_END sentinel
    for (
      uint8_t i = 0 ;
      !(
        map[i].input == Key_NoKey
        && map[i].output == Key_NoKey
        && map[i].timeout == 0
      ) ;
      ++i
    ) {

      if (mapped_key == map[i].input) {
        //Only activate this as part of the mapping if there isn't already a
        //key waiting for timeout. This allows us to return OK later and for
        //this loop to inject all the other flagged keys
        if (!other_mapped_key_flagged) {
          //The keypress was valid and a match. Mark it as flagged and reset the counter
          map[i].flagged = true;
          map[i].start_time = Runtime.millisAtCycleStart();

          //yes, we found a valid key
          valid_key = true;
        }
      } else {
        //If the key entry we're looking at was flagged previously, add it to the
        //report before we do anything else (this handles the situation where we
        //hit another key after this -- if it's a modifier, we want the modifier
        //key to be added to the report, for things like ctrl, alt, shift, etc)
        if (map[i].flagged) {
          handleKeyswitchEvent(map[i].input, UnknownKeyswitchLocation, IS_PRESSED | INJECTED);
        }

        //The keypress wasn't a match, so we need to mark it as not flagged and
        //reset the timer for it to disable everything.
        map[i].flagged = false;
        map[i].start_time = 0;
      }
    }

    //If we found a valid key in our map, we don't actually want to send anything.
    //This gets around an issue in Windows if we map a SpaceCadet function on top
    //of Alt -- sending Alt by itself activates the menubar.  We don't want to send
    //anything until we know that we're either sending the alternate key or we
    //know for sure that we want to send the originally pressed key.
    if (valid_key) {
      return EventHandlerResult::EVENT_CONSUMED;
    }

    //this is all we need to do on keypress, let the next handler do its thing too.
    //This case assumes we weren't a valid key that we were watching, so we don't
    //need to do anything else.
    return EventHandlerResult::OK;
  }

  // if the state is empty, that means that either an activating key wasn't pressed,
  // or we used another key in the interim. in both cases, nothing special to do.
  bool valid_key = false;
  bool pressed_key_was_valid = false;
  uint8_t index = 0;

  //Look to see if any keys in our map are currently flagged.
  //Exit condition is if we reach the special SPACECADET_MAP_END sentinel
  for (
    uint8_t i = 0 ;
    !(
      map[i].input == Key_NoKey
      && map[i].output == Key_NoKey
      && map[i].timeout == 0
    );
    ++i
  ) {

    //The key we're looking at was previously flagged (so perform action)
    if (map[i].flagged) {
      valid_key = true;
      index = i;
    }

    //the key we're looking at was valid (in the map)
    if (map[i].input == mapped_key) {
      pressed_key_was_valid = true;
    }
  }

  //If no valid mapped keys were pressed, simply return the key that
  //was originally passed in.
  if (!valid_key) {
    return EventHandlerResult::OK;
  }

  //use the map index to find the local timeout for this key
  uint16_t current_timeout = map[index].timeout;
  //If that isn't set, use the global timeout setting.
  if (current_timeout == 0) {
    current_timeout = time_out;
  }

  //Check to determine if we have surpassed our timeout for holding this key
  if (Runtime.hasTimeExpired(map[index].start_time, current_timeout)) {
    // if we timed out, that means we need to keep pressing the mapped
    // key, but we won't need to send the alternative key in the end
    map[index].flagged = false;
    map[index].start_time = 0;

    //Just return this key itself (we won't run alternative keys check)
    return EventHandlerResult::OK;
  }

  // If the key that was pressed isn't one of our mapped keys, just
  // return. This can happen when another key is released, and that should not
  // interrupt us.
  if (!pressed_key_was_valid) {
    return EventHandlerResult::OK;
  }

  // if a key toggled off (and that must be one of the mapped keys at this point),
  // send the alternative key instead (if we were interrupted, we bailed out earlier).
  if (keyToggledOff(key_state)) {
    Key alternate_key = map[index].output;

    //Since we are sending the actual key (no need for shift, etc),
    //only need to send that key and not the original key.

    //inject our new key
    handleKeyswitchEvent(alternate_key, key_addr, IS_PRESSED | INJECTED);

    //Unflag the key so we don't try this again.
    map[index].flagged = false;
    map[index].start_time = 0;
  }

  //Special case here for if we had a valid key that's continuing to be held.
  //If it's a valid key, and it's continuing to be held, return NoKey.
  //This prevents us from accidentally triggering a keypress that we didn't
  //mean to handle.
  if (valid_key) {
    return EventHandlerResult::EVENT_CONSUMED;
  }

  //Finally, as a final sanity check, simply return the passed-in key as-is.
  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::SpaceCadet SpaceCadet;

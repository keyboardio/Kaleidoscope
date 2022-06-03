/* -*- mode: c++ -*-
 * Kaleidoscope-GhostInTheFirmware -- Let the keyboard write for you!
 * Copyright (C) 2017  Keyboard.io, Inc
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

#include "kaleidoscope/plugin/GhostInTheFirmware.h"

#include <stdint.h>  // for uint16_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/keyswitch_state.h"       // for IS_PRESSED, WAS_PRESSED
#include "kaleidoscope/progmem_helpers.h"       // for loadFromProgmem

namespace kaleidoscope {
namespace plugin {

void GhostInTheFirmware::activate() {
  is_active_ = true;
}

EventHandlerResult GhostInTheFirmware::afterEachCycle() {
  if (!is_active_)
    return EventHandlerResult::OK;

  // This stores the current GhostKey in the active sequence.
  static GhostKey ghost_key{KeyAddr::none(), 0, 0};

  // When a ghost key has finished playing, it sets its delay to zero,
  // indicating that it's time to read the next one from memory.
  if (ghost_key.delay == 0) {
    // Read the settings for the key from PROGMEM:
    loadFromProgmem(ghost_keys[current_pos_], ghost_key);
    // The end of the sequence is marked by a GhostKey with an invalid KeyAddr
    // value (i.e. KeyAddr::none()). If we read this sentinel value, reset and
    // deactivate.
    if (!ghost_key.addr.isValid()) {
      current_pos_    = 0;
      ghost_key.delay = 0;
      is_active_      = false;
      return EventHandlerResult::OK;
    }
    // If we're not at the end of the sequence, send the first keypress event,
    // and start the timer.
    Runtime.handleKeyEvent(KeyEvent(ghost_key.addr, IS_PRESSED));
    start_time_ = Runtime.millisAtCycleStart();

  } else if (ghost_key.addr.isValid()) {
    // If the ghost key's address is still valid, that means that the virtual
    // key is still being held.
    if (Runtime.hasTimeExpired(start_time_, ghost_key.press_time)) {
      // The key press has timed out, so we send the release event.
      Runtime.handleKeyEvent(KeyEvent(ghost_key.addr, WAS_PRESSED));
      // Next, we invalidate the ghost key's address to prevent checking the
      // hold timeout again, then restart the timer for checking the delay.
      ghost_key.addr.clear();
      start_time_ = Runtime.millisAtCycleStart();
    }

  } else if (Runtime.hasTimeExpired(start_time_, ghost_key.delay)) {
    // The ghost key has been (virtually) pressed and released, and its delay
    // has now elapsed, so we set the delay to zero and increment the index
    // value to indicate that the next key should be loaded in the next cycle.
    ghost_key.delay = 0;
    ++current_pos_;
  }

  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::GhostInTheFirmware GhostInTheFirmware;

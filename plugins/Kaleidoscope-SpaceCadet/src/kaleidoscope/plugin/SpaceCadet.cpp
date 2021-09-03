/* -*- mode: c++ -*-
 * Kaleidoscope-SpaceCadet -- Space Cadet Shift Extended
 * Copyright (C) 2016, 2017, 2018  Keyboard.io, Inc, Ben Gemperline
 * Copyright (C) 2019-2021  Keyboard.io, Inc.
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
#include <Kaleidoscope-FocusSerial.h>
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/key_events.h"

//#include <Kaleidoscope-Devel-ArduinoTrace.h>

namespace kaleidoscope {
namespace plugin {

// Constructor with input and output, and assume default timeout
SpaceCadet::KeyBinding::KeyBinding(Key input, Key output)
  : input(input), output(output) {}

// Constructor with all three set
SpaceCadet::KeyBinding::KeyBinding(Key input, Key output, uint16_t timeout)
  : input(input), output(output), timeout(timeout) {}

// =============================================================================
// Space Cadet class variables

// -----------------------------------------------------------------------------
// Plugin configuration variables

SpaceCadet::KeyBinding * SpaceCadet::map;
uint16_t SpaceCadet::time_out = 200;

// -----------------------------------------------------------------------------
// State variables

uint8_t SpaceCadet::mode_ = SpaceCadet::Mode::ON;

// These variables are used to keep track of any pending unresolved SpaceCadet
// key that has been pressed. If `pending_map_index_` is negative, it means
// there is no such pending keypress. Otherwise, it holds the value of the index
// of that key in the array.
int8_t SpaceCadet::pending_map_index_ = -1;

KeyEventTracker SpaceCadet::event_tracker_;

// =============================================================================
// SpaceCadet functions

// Constructor
SpaceCadet::SpaceCadet() {
  static SpaceCadet::KeyBinding initialmap[] = {
    // By default, respect the default timeout
    {Key_LeftShift, Key_LeftParen, 0},
    {Key_RightShift, Key_RightParen, 0},
    // These may be uncommented, added, or set in the main sketch
    /*
    {Key_LeftGui, Key_LeftCurlyBracket, 250},
    {Key_RightAlt, Key_RightCurlyBracket, 250},
    {Key_LeftControl, Key_LeftBracket, 250},
    {Key_RightControl, Key_RightBracket, 250},
    */
    SPACECADET_MAP_END
  };

  map = initialmap;
}

// =============================================================================
// Event handler hook functions

// -----------------------------------------------------------------------------
EventHandlerResult SpaceCadet::onNameQuery() {
  return ::Focus.sendName(F("SpaceCadet"));
}

// -----------------------------------------------------------------------------
EventHandlerResult SpaceCadet::onKeyswitchEvent(KeyEvent &event) {
  // If SpaceCadet has already processed and released this event, ignore
  // it. There's no need to update the event tracker in this case.
  if (event_tracker_.shouldIgnore(event)) {
    // We should never get an event that's in our queue here, but just in case
    // some other plugin sends one, abort.
    if (event_queue_.shouldAbort(event))
      return EventHandlerResult::ABORT;
    return EventHandlerResult::OK;
  }

  // If event.addr is not a physical key, ignore it; some other plugin injected
  // it. This check should be unnecessary.
  if (!event.addr.isValid() || keyIsInjected(event.state)) {
    return EventHandlerResult::OK;
  }

  // Turn SpaceCadet on or off.
  if (keyToggledOn(event.state)) {
    if (event.key == Key_SpaceCadetEnable) {
      enable();
      return EventHandlerResult::EVENT_CONSUMED;
    }
    if (event.key == Key_SpaceCadetDisable) {
      disable();
      return EventHandlerResult::EVENT_CONSUMED;
    }
  }

  // Do nothing if disabled, but keep the event tracker current.
  if (mode_ == Mode::OFF)
    return EventHandlerResult::OK;

  if (!event_queue_.isEmpty()) {
    // There's an unresolved SpaceCadet key press.
    if (keyToggledOff(event.state)) {
      if (event.addr == event_queue_.addr(0)) {
        // SpaceCadet key released before timing out; send the event with the
        // SpaceCadet key's alternate `Key` value before flushing the rest of
        // the queue (see below).
        flushEvent(true);
      } else if (!event_queue_.isFull()) {
        // Queue not full; add event and abort
        event_queue_.append(event);
        return EventHandlerResult::ABORT;
      }
    }
    // Either a new key was pressed, or the SpaceCadet key was released and has
    // been flushed (see above), or the queue is full and is about to overflow.
    // In all cases, we fulsh the whole queue now.
    flushQueue();
  }

  // Event queue is now empty
  if (keyToggledOn(event.state)) {
    // Check for a SpaceCadet key
    pending_map_index_ = getSpaceCadetKeyIndex(event.key);
    if (pending_map_index_ >= 0) {
      // A SpaceCadet key has just toggled on. First, if we're in no-delay mode,
      // we need to send the event unchanged (with the primary `Key` value),
      // bypassing other `onKeyswitchEvent()` handlers.
      if (mode_ == Mode::NO_DELAY)
        Runtime.handleKeyEvent(event);
      // Queue the press event and abort; this press event will be resolved
      // later.
      event_queue_.append(event);
      return EventHandlerResult::ABORT;
    }
  }

  return EventHandlerResult::OK;
}

// -----------------------------------------------------------------------------
EventHandlerResult SpaceCadet::afterEachCycle() {
  // If there's no pending event, return.
  if (event_queue_.isEmpty())
    return EventHandlerResult::OK;

  // Get timeout value for the pending key.
  uint16_t pending_timeout = time_out;
  if (map[pending_map_index_].timeout != 0)
    pending_timeout = map[pending_map_index_].timeout;
  uint16_t start_time = event_queue_.timestamp(0);

  if (Runtime.hasTimeExpired(start_time, pending_timeout)) {
    // The timer has expired; release the pending event unchanged.
    flushQueue();
  }
  return EventHandlerResult::OK;
}

// =============================================================================
// Private helper function(s)

int8_t SpaceCadet::getSpaceCadetKeyIndex(Key key) const {
  for (uint8_t i = 0; !map[i].isEmpty(); ++i) {
    if (map[i].input == key) {
      return i;
    }
  }
  return -1;
}

void SpaceCadet::flushQueue() {
  while (!event_queue_.isEmpty()) {
    flushEvent(false);
  }
}

void SpaceCadet::flushEvent(bool is_tap) {
  KeyEvent event = event_queue_.event(0);
  if (is_tap && pending_map_index_ >= 0) {
    // If we're in no-delay mode, we should first send the release of the
    // modifier key as a courtesy before sending the tap event.
    if (mode_ == Mode::NO_DELAY) {
      Runtime.handleKeyEvent(KeyEvent(event.addr, WAS_PRESSED));
    }
    event.key = map[pending_map_index_].output;
  }
  event_queue_.shift();
  Runtime.handleKeyswitchEvent(event);
}

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::SpaceCadet SpaceCadet;

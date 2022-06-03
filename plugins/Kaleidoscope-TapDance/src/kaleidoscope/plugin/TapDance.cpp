/* -*- mode: c++ -*-
 * Kaleidoscope-TapDance -- Tap-dance keys
 * Copyright (C) 2016-2021  Keyboard.io, Inc
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

#include "kaleidoscope/plugin/TapDance.h"

#include <Arduino.h>                   // for F, __FlashStringHelper
#include <Kaleidoscope-FocusSerial.h>  // for Focus, FocusSerial
#include <Kaleidoscope-Ranges.h>       // for TD_FIRST
#include <stdint.h>                    // for uint8_t, uint16_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr, MatrixAddr
#include "kaleidoscope/KeyAddrEventQueue.h"     // for KeyAddrEventQueue
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/KeyEventTracker.h"       // for KeyEventTracker
#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/keyswitch_state.h"       // for keyIsInjected, keyToggledOff
#include "kaleidoscope/layers.h"                // for Layer, Layer_

namespace kaleidoscope {
namespace plugin {

// --- config ---

#ifndef NDEPRECATED
uint16_t TapDance::time_out = 200;
#endif

// --- api ---
void TapDance::actionKeys(uint8_t tap_count,
                          ActionType action,
                          uint8_t max_keys,
                          const Key tap_keys[]) {
  if (event_queue_.isEmpty())
    return;

  if (tap_count > max_keys)
    tap_count = max_keys;

  KeyEvent event = event_queue_.event(0);
  event.key      = tap_keys[tap_count - 1].readFromProgmem();

  if (action == Interrupt || action == Timeout || action == Hold) {
    event_queue_.shift();
    Runtime.handleKeyswitchEvent(event);
  } else if (action == Tap && tap_count == max_keys) {
    tap_count_ = 0;
    event_queue_.clear();
    Runtime.handleKeyswitchEvent(event);
  }
}


void TapDance::flushQueue(KeyAddr ignored_addr) {
  while (!event_queue_.isEmpty()) {
    KeyEvent queued_event = event_queue_.event(0);
    event_queue_.shift();
    if (queued_event.addr != ignored_addr)
      Runtime.handleKeyswitchEvent(queued_event);
  }
}

// --- hooks ---

EventHandlerResult TapDance::onNameQuery() {
  return ::Focus.sendName(F("TapDance"));
}

EventHandlerResult TapDance::onKeyswitchEvent(KeyEvent &event) {
  // If the plugin has already processed and released this event, ignore it.
  // There's no need to update the event tracker explicitly.
  if (event_tracker_.shouldIgnore(event)) {
    // We should never get an event that's in our queue here, but just in case
    // some other plugin sends one, abort.
    if (event_queue_.shouldAbort(event))
      return EventHandlerResult::ABORT;
    return EventHandlerResult::OK;
  }

  // If event.addr is not a physical key, ignore it; some other plugin injected it.
  if (!event.addr.isValid() || keyIsInjected(event.state)) {
    return EventHandlerResult::OK;
  }

  if (keyToggledOff(event.state)) {
    if (event_queue_.isEmpty())
      return EventHandlerResult::OK;
    event_queue_.append(event);
    return EventHandlerResult::ABORT;
  }

  if (event_queue_.isEmpty() && !isTapDanceKey(event.key))
    return EventHandlerResult::OK;

  KeyAddr td_addr = event_queue_.addr(0);
  Key td_key      = Layer.lookupOnActiveLayer(td_addr);
  uint8_t td_id   = td_key.getRaw() - ranges::TD_FIRST;

  if (!event_queue_.isEmpty() &&
      event.addr != event_queue_.addr(0)) {
    // Interrupt: Call `tapDanceAction()` first, so it will have access to the
    // TapDance key press event that needs to be sent, then flush the queue.
    tapDanceAction(td_id, td_addr, tap_count_, Interrupt);
    flushQueue();
    tap_count_ = 0;
    // If the event isn't another TapDance key, let it proceed. If it is, fall
    // through to the next block, which handles "Tap" actions.
    if (!isTapDanceKey(event.key))
      return EventHandlerResult::OK;
  }

  // Tap: First flush the queue, ignoring the previous press and release events
  // for the TapDance key, then add the new tap to the queue (it becomes the
  // first entry).
  flushQueue(event.addr);
  event_queue_.append(event);
  tapDanceAction(td_id, td_addr, ++tap_count_, Tap);
  return EventHandlerResult::ABORT;
}

EventHandlerResult TapDance::afterEachCycle() {
  // If there's no active TapDance sequence, there's nothing to do.
  if (event_queue_.isEmpty())
    return EventHandlerResult::OK;

  // The first event in the queue is now guaranteed to be a TapDance key.
  KeyAddr td_addr = event_queue_.addr(0);
  Key td_key      = Layer.lookupOnActiveLayer(td_addr);
  uint8_t td_id   = td_key.getRaw() - ranges::TD_FIRST;

  // Check for timeout
  uint16_t start_time = event_queue_.timestamp(0);
  // To avoid confusing editors with unmatched braces, we use a temporary
  // boolean, until the deprecated code can be removed.
  bool timed_out = false;
#ifndef NDEPRECATED
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
  if (Runtime.hasTimeExpired(start_time, time_out))
    timed_out = true;
#pragma GCC diagnostic pop
#else
  if (Runtime.hasTimeExpired(start_time, timeout_))
    timed_out = true;
#endif
  if (timed_out) {
    // We start with the assumption that the TapDance key is still being held.
    ActionType action = Hold;
    // Now we search for a release event for the TapDance key, starting from the
    // second event in the queue (the first one being its press event).
    for (uint8_t i{1}; i < event_queue_.length(); ++i) {
      // It should be safe to assume that if we find a second event for the same
      // address, it's a release, so we skip the test for it.
      if (event_queue_.addr(i) == td_addr) {
        action = Timeout;
        // We don't need to bother breaking here because this is basically
        // guaranteed to be the last event in the queue.
      }
    }
    tapDanceAction(td_id, td_addr, tap_count_, action);
    flushQueue();
    tap_count_ = 0;
  }
  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

__attribute__((weak)) void tapDanceAction(uint8_t tap_dance_index, KeyAddr key_addr, uint8_t tap_count, kaleidoscope::plugin::TapDance::ActionType tap_dance_action) {
}

kaleidoscope::plugin::TapDance TapDance;

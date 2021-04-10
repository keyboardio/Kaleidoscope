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

#include <Kaleidoscope-TapDance.h>
#include <Kaleidoscope-FocusSerial.h>
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/layers.h"

namespace kaleidoscope {
namespace plugin {

// --- config ---

uint16_t TapDance::time_out = 200;
KeyAddr TapDance::release_addr_ = KeyAddr{KeyAddr::invalid_state};

// --- api ---
void TapDance::actionKeys(uint8_t tap_count,
                          ActionType tap_dance_action,
                          uint8_t max_keys,
                          const Key tap_keys[]) {
  if (tap_count > max_keys)
    tap_count = max_keys;

  Key key = tap_keys[tap_count - 1].readFromProgmem();

  switch (tap_dance_action) {
  case Tap:
    break;
  case Interrupt:
  case Timeout:
    if (event_queue_.isEmpty())
      break;
    {
      KeyAddr td_addr = event_queue_.addr(0);
      bool key_released = (live_keys[td_addr] == Key_Transparent);
      handleKeyswitchEvent(key, td_addr, IS_PRESSED | INJECTED);
      if (key_released)
        release_addr_ = td_addr;
    }
    break;
  case Hold:
  case Release:
    break;
  }
}


// --- hooks ---

EventHandlerResult TapDance::onNameQuery() {
  return ::Focus.sendName(F("TapDance"));
}

EventHandlerResult TapDance::onKeyswitchEvent(Key &key,
                                              KeyAddr key_addr,
                                              uint8_t key_state) {
  if (key_state & INJECTED)
    return EventHandlerResult::OK;

  if (event_queue_.isEmpty()) {
    if (keyToggledOn(key_state) && isTapDanceKey(key)) {
      // Begin a new TapDance sequence:
      uint8_t td_id = key.getRaw() - ranges::TD_FIRST;
      tapDanceAction(td_id, key_addr, 1, Tap);
      event_queue_.append(key_addr, key_state);
      return EventHandlerResult::EVENT_CONSUMED;
    }
    return EventHandlerResult::OK;
  }

  uint8_t td_count = event_queue_.length();
  KeyAddr td_addr = event_queue_.addr(0);
  Key     td_key = Layer.lookup(td_addr);
  uint8_t td_id = td_key.getRaw() - ranges::TD_FIRST;

  if (keyToggledOn(key_state)) {
    if (key_addr == td_addr) {
      // The same TapDance key was pressed again; continue the sequence:
      tapDanceAction(td_id, td_addr, ++td_count, Tap);
    } else {
      // A different key was pressed; interrupt the sequeunce:
      tapDanceAction(td_id, td_addr, td_count, Interrupt);
      event_queue_.clear();
      // If the sequence was interrupted by another TapDance key, start the new
      // sequence:
      if (isTapDanceKey(Layer.lookup(key_addr))) {
        td_id = key.getRaw() - ranges::TD_FIRST;
        tapDanceAction(td_id, key_addr, 1, Tap);
      }
    }
    // Any key that toggles on while a TapDance sequence is live gets added to
    // the queue. If it interrupted the queue, we need to hold off on processing
    // that event until the next cycle to guarantee that the events appear in
    // order on the host.
    event_queue_.append(key_addr, key_state);
    if (isTapDanceKey(key))
      return EventHandlerResult::EVENT_CONSUMED;
    return EventHandlerResult::ABORT;
  } else if (keyIsPressed(key_state)) {
    // Until a key press event has been released from the queue, its "hold
    // event" must be suppressed every cycle.
    for (uint8_t i{0}; i < event_queue_.length(); ++i) {
      if (event_queue_.addr(i) == key_addr) {
        return EventHandlerResult::ABORT;
      }
    }
  }
  // We always indicate that other plugins don't need to handle TapDance keys,
  // but we do allow them to show up as active keys when they're held. This way,
  // when one times out, if it's not being held any longer, we can send the
  // release event.
  if (isTapDanceKey(key))
    return EventHandlerResult::EVENT_CONSUMED;
  // This key is being held, but is not in the queue, or it toggled off, but is
  // not (currently) a TapDance key.
  return EventHandlerResult::OK;
}

EventHandlerResult TapDance::afterEachCycle() {
  if (release_addr_.isValid()) {
    handleKeyswitchEvent(Key_NoKey, release_addr_, WAS_PRESSED | INJECTED);
    release_addr_ = KeyAddr{KeyAddr::invalid_state};
  }
  Key event_key;
  // Purge any non-TapDance key events from the front of the queue.
  while (! event_queue_.isEmpty()) {
    KeyAddr event_addr = event_queue_.addr(0);
    event_key = Layer.lookup(event_addr);
    if (isTapDanceKey(event_key)) {
      break;
    }
    handleKeyswitchEvent(event_key, event_addr, IS_PRESSED | INJECTED);
    event_queue_.shift();
  }

  if (event_queue_.isEmpty())
    return EventHandlerResult::OK;

  // The first event in the queue is now guaranteed to be a TapDance key.
  uint8_t td_id = event_key.getRaw() - ranges::TD_FIRST;
  KeyAddr td_addr = event_queue_.addr(0);

  // Check for timeout
  uint8_t td_count = event_queue_.length();
  uint16_t start_time = event_queue_.timestamp(td_count - 1);
  if (Runtime.hasTimeExpired(start_time, time_out)) {
    tapDanceAction(td_id, td_addr, td_count, Timeout);
    event_queue_.clear();
    // There's still a race condition here, but it's a minor one. If a TapDance
    // sequence times out in the `afterEachCycle()` handler, then another key
    // toggles on in the following scan cycle, and that key is handled first,
    // the two events could show up out of order on the host. The probability of
    // this happening is low, and event-driven Kaleidoscope will fix it
    // completely, so I'm willing to accept the risk for now.
  }
  return EventHandlerResult::OK;
}

}
}

__attribute__((weak)) void tapDanceAction(uint8_t tap_dance_index, KeyAddr key_addr, uint8_t tap_count,
                                          kaleidoscope::plugin::TapDance::ActionType tap_dance_action) {
}

kaleidoscope::plugin::TapDance TapDance;

/* -*- mode: c++ -*-
 * Kaleidoscope-Leader -- VIM-style leader keys
 * Copyright (C) 2016, 2017, 2018  Keyboard.io, Inc
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

#include <Kaleidoscope-Leader.h>
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/key_events.h"

namespace kaleidoscope {
namespace plugin {

// --- state ---
Key Leader::sequence_[LEADER_MAX_SEQUENCE_LENGTH + 1];
uint8_t Leader::sequence_pos_;
uint16_t Leader::start_time_ = 0;
uint16_t Leader::time_out = 1000;
const Leader::dictionary_t *Leader::dictionary;

// --- helpers ---

#define PARTIAL_MATCH -1
#define NO_MATCH -2

#define isLeader(k) (k.getRaw() >= ranges::LEAD_FIRST && k.getRaw() <= ranges::LEAD_LAST)
#define isActive() (sequence_[0] != Key_NoKey)

// --- actions ---
int8_t Leader::lookup(void) {
  bool match;

  for (uint8_t seq_index = 0; ; seq_index++) {
    match = true;

    if (dictionary[seq_index].sequence[0].readFromProgmem() == Key_NoKey)
      break;

    Key seq_key;
    for (uint8_t i = 0; i <= sequence_pos_; i++) {
      seq_key = dictionary[seq_index].sequence[i].readFromProgmem();

      if (sequence_[i] != seq_key) {
        match = false;
        break;
      }
    }

    if (!match)
      continue;

    seq_key
      = dictionary[seq_index].sequence[sequence_pos_ + 1].readFromProgmem();
    if (seq_key == Key_NoKey) {
      return seq_index;
    } else {
      return PARTIAL_MATCH;
    }
  }

  return NO_MATCH;
}

// --- api ---

void Leader::reset(void) {
  sequence_pos_ = 0;
  sequence_[0] = Key_NoKey;
}

void Leader::inject(Key key, uint8_t key_state) {
  onKeyswitchEvent(key, UnknownKeyswitchLocation, key_state);
}

// --- hooks ---
EventHandlerResult Leader::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t keyState) {
  if (keyState & INJECTED)
    return EventHandlerResult::OK;

  if (!isActive() && !isLeader(mapped_key))
    return EventHandlerResult::OK;

  if (!isActive()) {
    // Must be a leader key!

    if (keyToggledOff(keyState)) {
      // not active, but a leader key = start the sequence on key release!
      start_time_ = Runtime.millisAtCycleStart();
      sequence_pos_ = 0;
      sequence_[sequence_pos_] = mapped_key;
    }

    // If the sequence was not active yet, ignore the key.
    return EventHandlerResult::EVENT_CONSUMED;
  }

  // active
  int8_t action_index = lookup();

  if (keyToggledOn(keyState)) {
    sequence_pos_++;
    if (sequence_pos_ > LEADER_MAX_SEQUENCE_LENGTH) {
      reset();
      return EventHandlerResult::OK;
    }

    start_time_ = Runtime.millisAtCycleStart();
    sequence_[sequence_pos_] = mapped_key;
    action_index = lookup();

    if (action_index >= 0) {
      return EventHandlerResult::EVENT_CONSUMED;
    }
  } else if (keyIsPressed(keyState)) {
    // held, no need for anything here.
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (action_index == NO_MATCH) {
    reset();
    return EventHandlerResult::OK;
  }
  if (action_index == PARTIAL_MATCH) {
    return EventHandlerResult::EVENT_CONSUMED;
  }

  action_t leaderAction = (action_t) pgm_read_ptr((void const **) & (dictionary[action_index].action));
  (*leaderAction)(action_index);

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult Leader::afterEachCycle() {
  if (!isActive())
    return EventHandlerResult::OK;

  if (Runtime.hasTimeExpired(start_time_, time_out))
    reset();

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::Leader Leader;

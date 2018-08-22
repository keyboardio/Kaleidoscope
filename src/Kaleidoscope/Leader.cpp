/* -*- mode: c++ -*-
 * Kaleidoscope-Leader -- VIM-style leader keys
 * Copyright (C) 2016, 2017, 2018  Gergely Nagy
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

namespace kaleidoscope {

// --- state ---
Key Leader::sequence_[LEADER_MAX_SEQUENCE_LENGTH + 1];
uint8_t Leader::sequence_pos_;
uint32_t Leader::end_time_;
uint16_t Leader::time_out = 1000;
const Leader::dictionary_t *Leader::dictionary;

// --- helpers ---

#define PARTIAL_MATCH -1
#define NO_MATCH -2

#define isLeader(k) (k.raw >= ranges::LEAD_FIRST && k.raw <= ranges::LEAD_LAST)
#define isActive() (sequence_[0].raw != Key_NoKey.raw)

// --- actions ---
int8_t Leader::lookup(void) {
  bool match;

  for (uint8_t seq_index = 0; ; seq_index++) {
    match = true;

    if (pgm_read_word(&(dictionary[seq_index].sequence[0].raw)) == Key_NoKey.raw)
      break;

    Key seq_key;
    for (uint8_t i = 0; i <= sequence_pos_; i++) {
      seq_key.raw = pgm_read_word(&(dictionary[seq_index].sequence[i].raw));

      if (sequence_[i].raw != seq_key.raw) {
        match = false;
        break;
      }
    }

    if (!match)
      continue;

    seq_key.raw = pgm_read_word(&(dictionary[seq_index].sequence[sequence_pos_ + 1].raw));
    if (seq_key.raw == Key_NoKey.raw) {
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
  sequence_[0].raw = Key_NoKey.raw;
}

void Leader::inject(Key key, uint8_t key_state) {
  onKeyswitchEvent(key, UNKNOWN_KEYSWITCH_LOCATION, key_state);
}

// --- hooks ---
EventHandlerResult Leader::onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t keyState) {
  if (keyState & INJECTED)
    return EventHandlerResult::OK;

  if (!keyIsPressed(keyState) && !keyWasPressed(keyState)) {
    if (isLeader(mapped_key)) {
      return EventHandlerResult::EVENT_CONSUMED;
    }
    return EventHandlerResult::OK;
  }

  if (!isActive() && !isLeader(mapped_key))
    return EventHandlerResult::OK;

  if (!isActive()) {
    // Must be a leader key!

    if (keyToggledOff(keyState)) {
      // not active, but a leader key = start the sequence on key release!
      end_time_ = millis() + time_out;
      sequence_pos_ = 0;
      sequence_[sequence_pos_].raw = mapped_key.raw;
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

    end_time_ = millis() + time_out;
    sequence_[sequence_pos_].raw = mapped_key.raw;
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

  action_t leaderAction = (action_t) pgm_read_ptr(&(dictionary[action_index].action));
  (*leaderAction)(action_index);

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult Leader::afterEachCycle() {
  if (!isActive())
    return EventHandlerResult::OK;

  if (millis() >= end_time_)
    reset();

  return EventHandlerResult::OK;
}

}

kaleidoscope::Leader Leader;

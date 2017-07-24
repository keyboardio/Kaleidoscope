/* -*- mode: c++ -*-
 * Kaleidoscope-Leader -- VIM-style leader keys
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

Leader::Leader(void) {
}

void Leader::begin(void) {
  event_handler_hook_use(eventHandlerHook);
  loop_hook_use(loopHook);
}

void Leader::reset(void) {
  sequence_pos_ = 0;
  sequence_[0].raw = Key_NoKey.raw;
}

void Leader::inject(Key key, uint8_t key_state) {
  eventHandlerHook(key, UNKNOWN_KEYSWITCH_LOCATION, key_state);
}

// --- hooks ---
Key Leader::eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state) {
  if (key_state & INJECTED)
    return mapped_key;

  if (!keyIsPressed(key_state) && !keyWasPressed(key_state)) {
    if (isLeader(mapped_key))
      return Key_NoKey;
    return mapped_key;
  }

  if (!isActive() && !isLeader(mapped_key))
    return mapped_key;

  if (!isActive()) {
    // Must be a leader key!

    if (keyToggledOff(key_state)) {
      // not active, but a leader key = start the sequence on key release!
      end_time_ = millis() + time_out;
      sequence_pos_ = 0;
      sequence_[sequence_pos_].raw = mapped_key.raw;
    }

    // If the sequence was not active yet, ignore the key.
    return Key_NoKey;
  }

  // active
  int8_t action_index = lookup();

  if (keyToggledOn(key_state)) {
    sequence_pos_++;
    if (sequence_pos_ > LEADER_MAX_SEQUENCE_LENGTH) {
      reset();
      return mapped_key;
    }

    end_time_ = millis() + time_out;
    sequence_[sequence_pos_].raw = mapped_key.raw;
    action_index = lookup();

    if (action_index >= 0)
      return Key_NoKey;
  } else if (keyIsPressed(key_state)) {
    // held, no need for anything here.
    return Key_NoKey;
  }

  if (action_index == NO_MATCH) {
    reset();
    return mapped_key;
  }
  if (action_index == PARTIAL_MATCH) {
    return Key_NoKey;
  }

  action_t leaderAction = (action_t) pgm_read_ptr(&(dictionary[action_index].action));
  (*leaderAction)(action_index);
  return Key_NoKey;
}

void Leader::loopHook(bool is_post_clear) {
  if (!is_post_clear)
    return;

  if (!isActive())
    return;

  if (millis() >= end_time_)
    reset();
}

}

kaleidoscope::Leader Leader;

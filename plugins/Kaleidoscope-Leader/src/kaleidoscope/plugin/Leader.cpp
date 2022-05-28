/* -*- mode: c++ -*-
 * Kaleidoscope-Leader -- VIM-style leader keys
 * Copyright (C) 2016, 2017, 2018, 2021  Keyboard.io, Inc
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

#include "kaleidoscope/plugin/Leader.h"

#include <Arduino.h>                   // for F, __FlashStringHelper, pgm_read_ptr
#include <Kaleidoscope-FocusSerial.h>  // for Focus, FocusSerial
#include <Kaleidoscope-Ranges.h>       // for LEAD_FIRST, LEAD_LAST
#include <stdint.h>                    // for uint16_t, uint8_t, int8_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/KeyEventTracker.h"       // for KeyEventTracker
#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/key_defs.h"              // for Key, Key_NoKey
#include "kaleidoscope/keyswitch_state.h"       // for INJECTED, keyToggledOff

namespace kaleidoscope {
namespace plugin {

// --- state ---
#ifndef NDEPRECATED
uint16_t Leader::time_out = 1000;
#endif

// --- helpers ---

#define PARTIAL_MATCH -1
#define NO_MATCH      -2

#define isLeader(k)   (k.getRaw() >= ranges::LEAD_FIRST && k.getRaw() <= ranges::LEAD_LAST)
#define isActive()    (sequence_[0] != Key_NoKey)

// --- actions ---
int8_t Leader::lookup() {
  bool match;

  for (uint8_t seq_index = 0;; seq_index++) {
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

    seq_key = dictionary[seq_index].sequence[sequence_pos_ + 1].readFromProgmem();
    if (seq_key == Key_NoKey) {
      return seq_index;
    } else {
      return PARTIAL_MATCH;
    }
  }

  return NO_MATCH;
}

// --- api ---

void Leader::reset() {
  sequence_pos_ = 0;
  sequence_[0]  = Key_NoKey;
}

#ifndef NDEPRECATED
void Leader::inject(Key key, uint8_t key_state) {
  Runtime.handleKeyEvent(KeyEvent(KeyAddr::none(), key_state | INJECTED, key));
}
#endif

// --- hooks ---
EventHandlerResult Leader::onNameQuery() {
  return ::Focus.sendName(F("Leader"));
}

EventHandlerResult Leader::onKeyswitchEvent(KeyEvent &event) {
  // If the plugin has already processed and released this event, ignore it.
  // There's no need to update the event tracker explicitly.
  if (event_tracker_.shouldIgnore(event))
    return EventHandlerResult::OK;

  if (keyToggledOff(event.state) || event.state & INJECTED)
    return EventHandlerResult::OK;

  if (!isActive()) {
    if (!isLeader(event.key))
      return EventHandlerResult::OK;

    start_time_              = Runtime.millisAtCycleStart();
    sequence_pos_            = 0;
    sequence_[sequence_pos_] = event.key;

    return EventHandlerResult::ABORT;
  }

  ++sequence_pos_;
  if (sequence_pos_ > LEADER_MAX_SEQUENCE_LENGTH) {
    reset();
    return EventHandlerResult::OK;
  }

  start_time_              = Runtime.millisAtCycleStart();
  sequence_[sequence_pos_] = event.key;
  int8_t action_index      = lookup();

  if (action_index == NO_MATCH) {
    reset();
    return EventHandlerResult::OK;
  }
  if (action_index == PARTIAL_MATCH) {
    return EventHandlerResult::ABORT;
  }

  action_t leaderAction = (action_t)pgm_read_ptr((void const **)&(dictionary[action_index].action));
  (*leaderAction)(action_index);
  reset();

  return EventHandlerResult::ABORT;
}

EventHandlerResult Leader::afterEachCycle() {
  if (!isActive())
    return EventHandlerResult::OK;

#ifndef NDEPRECATED
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
  if (Runtime.hasTimeExpired(start_time_, time_out))
    reset();
#pragma GCC diagnostic pop
#else
  if (Runtime.hasTimeExpired(start_time_, timeout_))
    reset();
#endif

  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::Leader Leader;

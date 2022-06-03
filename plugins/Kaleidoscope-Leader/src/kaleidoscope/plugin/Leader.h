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

#pragma once

#include <Kaleidoscope-Ranges.h>  // for LEAD_FIRST
#include <stddef.h>               // for NULL
#include <stdint.h>               // for uint16_t, uint8_t, int8_t

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/KeyEventTracker.h"       // for KeyEventTracker
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key, Key_NoKey
#include "kaleidoscope/plugin.h"                // for Plugin
// -----------------------------------------------------------------------------
// Deprecation warning messages
#include "kaleidoscope_internal/deprecations.h"  // for DEPRECATED

#define _DEPRECATED_MESSAGE_LEADER_INJECT                        \
  "The `Leader.inject()` function is deprecated. Please call\n"  \
  "`kaleidoscope::Runtime.handleKeyEvent()` directly instead.\n" \
  "This function will be removed after 2022-09-01."

#define _DEPRECATED_MESSAGE_LEADER_TIME_OUT                        \
  "The `Leader.time_out` variable is deprecated. Please use the\n" \
  "`Leader.setTimeout()` function instead.\n"                      \
  "This variable will be removed after 2022-09-01."
// -----------------------------------------------------------------------------

#define LEADER_MAX_SEQUENCE_LENGTH 4

#define LEAD(n)                    kaleidoscope::plugin::LeaderKey(n)

#define LEADER_SEQ(...) \
  { __VA_ARGS__, Key_NoKey }
#define LEADER_DICT(...)               \
  {                                    \
    __VA_ARGS__, { {Key_NoKey}, NULL } \
  }

namespace kaleidoscope {
namespace plugin {

constexpr Key LeaderKey(uint8_t n) {
  return Key(kaleidoscope::ranges::LEAD_FIRST + n);
}

class Leader : public kaleidoscope::Plugin {
 public:
  typedef void (*action_t)(uint8_t seq_index);
  struct dictionary_t {
    Key sequence[LEADER_MAX_SEQUENCE_LENGTH + 1];
    action_t action;
  };

  const dictionary_t *dictionary;

  void reset();

#ifndef NDEPRECATED
  DEPRECATED(LEADER_TIME_OUT)
  static uint16_t time_out;

  DEPRECATED(LEADER_INJECT)
  void inject(Key key, uint8_t key_state);
#endif

  void setTimeout(uint16_t timeout) {
#ifndef NDEPRECATED
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    time_out = timeout;
#pragma GCC diagnostic pop
#endif
    timeout_ = timeout;
  }

  EventHandlerResult onNameQuery();
  EventHandlerResult onKeyswitchEvent(KeyEvent &event);
  EventHandlerResult afterEachCycle();

 private:
  Key sequence_[LEADER_MAX_SEQUENCE_LENGTH + 1];
  KeyEventTracker event_tracker_;
  uint8_t sequence_pos_;
  uint16_t start_time_ = 0;
  uint16_t timeout_    = 1000;

  int8_t lookup();
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::Leader Leader;

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

#pragma once

#include "kaleidoscope/Runtime.h"
#include <Kaleidoscope-Ranges.h>

#define LEADER_MAX_SEQUENCE_LENGTH 4

#define LEAD(n) Key(kaleidoscope::ranges::LEAD_FIRST + n)

#define LEADER_SEQ(...) { __VA_ARGS__, Key_NoKey }
#define LEADER_DICT(...) { __VA_ARGS__, {{Key_NoKey}, NULL} }

namespace kaleidoscope {
namespace plugin {

class Leader : public kaleidoscope::Plugin {
 public:
  typedef void (*action_t)(uint8_t seq_index);
  typedef struct {
    Key sequence[LEADER_MAX_SEQUENCE_LENGTH + 1];
    action_t action;
  } dictionary_t;

  Leader(void) {}
  static const dictionary_t *dictionary;

  static void reset(void);
  static uint16_t time_out;

  void inject(Key key, uint8_t key_state);

  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t keyState);
  EventHandlerResult afterEachCycle();

 private:
  static Key sequence_[LEADER_MAX_SEQUENCE_LENGTH + 1];
  static uint8_t sequence_pos_;
  static uint16_t start_time_;

  static int8_t lookup(void);
};
}

}

extern kaleidoscope::plugin::Leader Leader;

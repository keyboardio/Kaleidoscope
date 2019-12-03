/* -*- mode: c++ -*-
 * Kaleidoscope-TypingBreaks -- Enforced typing breaks
 * Copyright (C) 2017, 2018, 2019  Keyboard.io, Inc
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

namespace kaleidoscope {
namespace plugin {

class TypingBreaks : public kaleidoscope::Plugin {
 public:
  TypingBreaks(void) {}

  typedef struct settings_t {
    uint16_t idle_time_limit;
    uint16_t lock_time_out;
    uint16_t lock_length;
    uint16_t left_hand_max_keys;
    uint16_t right_hand_max_keys;
  } settings_t;

  static settings_t settings;

  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state);
  EventHandlerResult onFocusEvent(const char *command);
  EventHandlerResult onSetup();

 private:
  static bool keyboard_locked_;
  static uint32_t session_start_time_;
  static uint32_t lock_start_time_;
  static uint32_t last_key_time_;
  static uint16_t left_hand_keys_;
  static uint16_t right_hand_keys_;

  static uint16_t settings_base_;
};
}
}

extern kaleidoscope::plugin::TypingBreaks TypingBreaks;

void TypingBreak(bool is_locked);

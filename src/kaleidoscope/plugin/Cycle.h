/* -*- mode: c++ -*-
 * Kaleidoscope-Cycle -- Key sequence cycling dead key for Kaleidoscope.
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

#define Key_Cycle Key(kaleidoscope::ranges::CYCLE)

#define cycleThrough(...) ({                                \
      static const Key __k[] PROGMEM = { __VA_ARGS__ };     \
      Cycle.replace(sizeof(__k) / sizeof(Key), &__k[0]); \
    })

namespace kaleidoscope {
namespace plugin {
class Cycle : public kaleidoscope::Plugin {
 public:
  Cycle(void) {}

  static void replace(Key key);
  static void replace(uint8_t cycle_size, const Key cycle_steps[]);

  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state);

 private:
  static uint8_t toModFlag(uint8_t keyCode);
  static Key last_non_cycle_key_;
  static uint8_t cycle_count_;
  static uint8_t current_modifier_flags_;
};
}
}

void cycleAction(Key previous_key, uint8_t cycle_count);

extern kaleidoscope::plugin::Cycle Cycle;

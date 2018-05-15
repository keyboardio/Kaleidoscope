/* -*- mode: c++ -*-
 * Kaleidoscope-Cycle -- Key sequence cycling dead key for Kaleidoscope.
 * Copyright (C) 2016, 2017, 2018  Gergely Nagy
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

#pragma once

#include <Kaleidoscope.h>
#include <Kaleidoscope-Ranges.h>

#define Key_Cycle ((Key) { .raw = kaleidoscope::ranges::CYCLE })

#define cycleThrough(...) ({                                \
      static const Key __k[] PROGMEM = { __VA_ARGS__ };     \
      Cycle.replace(sizeof(__k) / sizeof(Key), &__k[0]); \
    })

namespace kaleidoscope {
class Cycle : public kaleidoscope::Plugin {
 public:
  Cycle(void) {}

  static void replace(Key key);
  static void replace(uint8_t cycle_size, const Key cycle_steps[]);

  EventHandlerResult onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t key_state);

#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
 protected:
  void begin();
  static Key legacyEventHandler(Key mapped_key, byte row, byte col, uint8_t key_state);
#endif

 private:
  static Key last_non_cycle_key_;
  static uint8_t cycle_count_;
};
};

void cycleAction(Key previous_key, uint8_t cycle_count);

extern kaleidoscope::Cycle Cycle;

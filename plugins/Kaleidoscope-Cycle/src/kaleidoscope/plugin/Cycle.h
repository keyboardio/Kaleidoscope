/* -*- mode: c++ -*-
 * Kaleidoscope-Cycle -- Key sequence cycling dead key for Kaleidoscope.
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

#include <Kaleidoscope-Ranges.h>  // for CYCLE
#include <stdint.h>               // for uint8_t

#include "Arduino.h"                            // for PROGMEM
#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/plugin.h"                // for Plugin

constexpr Key Key_Cycle = Key(kaleidoscope::ranges::CYCLE);

#define cycleThrough(...) ({                         \
  static const Key __k[] PROGMEM = {__VA_ARGS__};    \
  Cycle.replace(sizeof(__k) / sizeof(Key), &__k[0]); \
})

namespace kaleidoscope {
namespace plugin {
class Cycle : public kaleidoscope::Plugin {
 public:
  void replace(Key key);
  void replace(uint8_t cycle_size, const Key cycle_steps[]);

  EventHandlerResult onNameQuery();
  EventHandlerResult onKeyEvent(KeyEvent &event);

 private:
  uint8_t toModFlag(uint8_t keyCode);
  Key last_non_cycle_key_;
  KeyAddr cycle_key_addr_{KeyAddr::invalid_state};
  uint8_t cycle_count_;
  uint8_t current_modifier_flags_;
};

}  // namespace plugin
}  // namespace kaleidoscope

void cycleAction(Key previous_key, uint8_t cycle_count);

extern kaleidoscope::plugin::Cycle Cycle;

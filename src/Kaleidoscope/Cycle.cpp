/* -*- mode: c++ -*-
 * Kaleidoscope-Cycle -- Key sequence cycling dead key for Kaleidoscope.
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

#include <Kaleidoscope.h>
#include <Kaleidoscope-Cycle.h>

namespace kaleidoscope {
// --- state ---
Key Cycle::last_non_cycle_key_;
uint8_t Cycle::cycle_count_;

// --- helpers ---

#define isCycle(k) (k.raw == kaleidoscope::ranges::CYCLE)

// --- api ---

Cycle::Cycle(void) {
}

void Cycle::begin(void) {
  event_handler_hook_use(eventHandlerHook);
}

void Cycle::replace(Key key) {
  handle_keyswitch_event(Key_Backspace, UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED | INJECTED);
  Keyboard.sendReport();
  handle_keyswitch_event(Key_Backspace, UNKNOWN_KEYSWITCH_LOCATION, WAS_PRESSED | INJECTED);
  Keyboard.sendReport();

  handle_keyswitch_event(key, UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED | INJECTED);
  Keyboard.sendReport();
  handle_keyswitch_event(key, UNKNOWN_KEYSWITCH_LOCATION, WAS_PRESSED | INJECTED);
  Keyboard.sendReport();
}

void Cycle::replace(uint8_t cycle_size, const Key cycle_steps[]) {
  uint8_t idx = cycle_count_ % cycle_size;
  Key key;

  key.raw = pgm_read_word(cycle_steps + idx);
  replace(key);
}

// --- hooks ---

Key Cycle::eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state) {
  if (key_state & INJECTED)
    return mapped_key;

  if (!key_is_pressed(key_state) && !key_was_pressed(key_state)) {
    if (isCycle(mapped_key))
      return Key_NoKey;
    return mapped_key;
  }

  if (!isCycle(mapped_key)) {
    if (keyToggledOn(key_state)) {
      last_non_cycle_key_.raw = mapped_key.raw;
      cycle_count_ = 0;
    }
    return mapped_key;
  }

  if (!keyToggledOff(key_state))
    return Key_NoKey;

  ++cycle_count_;
  cycleAction(last_non_cycle_key_, cycle_count_);
  return Key_NoKey;
}
};

__attribute__((weak))
void cycleAction(Key previous_key, uint8_t cycle_count) {
}

kaleidoscope::Cycle Cycle;

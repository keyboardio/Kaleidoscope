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

#include <Kaleidoscope.h>
#include <Kaleidoscope-Cycle.h>
#include "kaleidoscope/hid.h"

namespace kaleidoscope {
namespace plugin {
// --- state ---
Key Cycle::last_non_cycle_key_;
uint8_t Cycle::current_modifier_flags_;
uint8_t Cycle::cycle_count_;

// --- helpers ---

#define isCycle(k) (k.raw == kaleidoscope::ranges::CYCLE)

// --- api ---

void Cycle::replace(Key key) {
  handleKeyswitchEvent(Key_Backspace, UnknownKeyswitchLocation, IS_PRESSED | INJECTED);
  hid::sendKeyboardReport();
  handleKeyswitchEvent(Key_Backspace, UnknownKeyswitchLocation, WAS_PRESSED | INJECTED);
  hid::sendKeyboardReport();

  handleKeyswitchEvent(key, UnknownKeyswitchLocation, IS_PRESSED | INJECTED);
  hid::sendKeyboardReport();
  handleKeyswitchEvent(key, UnknownKeyswitchLocation, WAS_PRESSED | INJECTED);
  hid::sendKeyboardReport();
}

void Cycle::replace(uint8_t cycle_size, const Key cycle_steps[]) {
  uint8_t idx = cycle_count_ % cycle_size;
  Key key;

  key.raw = pgm_read_word(cycle_steps + idx);
  replace(key);
}

// --- hooks ---

EventHandlerResult Cycle::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state) {
  if (key_state & INJECTED)
    return EventHandlerResult::OK;

  if (!keyIsPressed(key_state) && !keyWasPressed(key_state)) {
    if (isCycle(mapped_key)) {
      return EventHandlerResult::EVENT_CONSUMED;
    }
    return EventHandlerResult::OK;
  }

  if (!isCycle(mapped_key)) {
    if (keyToggledOn(key_state)) {
      current_modifier_flags_ |= toModFlag(mapped_key.keyCode);
      last_non_cycle_key_.keyCode = mapped_key.keyCode;
      last_non_cycle_key_.flags = current_modifier_flags_;
      cycle_count_ = 0;
    }
    if (keyToggledOff(key_state)) {
      current_modifier_flags_ &= ~toModFlag(mapped_key.keyCode);
    }
    return EventHandlerResult::OK;
  }

  if (!keyToggledOff(key_state)) {
    return EventHandlerResult::EVENT_CONSUMED;
  }

  ++cycle_count_;
  cycleAction(last_non_cycle_key_, cycle_count_);
  return EventHandlerResult::EVENT_CONSUMED;
}

uint8_t Cycle::toModFlag(uint8_t keyCode) {
  switch (keyCode) {
  case Key_LeftShift.keyCode:
  case Key_RightShift.keyCode:
    return SHIFT_HELD;
  case Key_LeftAlt.keyCode:
    return LALT_HELD;
  case Key_RightAlt.keyCode:
    return RALT_HELD;
  case Key_LeftControl.keyCode:
  case Key_RightControl.keyCode:
    return CTRL_HELD;
  case Key_LeftGui.keyCode:
  case Key_RightGui.keyCode:
    return GUI_HELD;
  default:
    return 0;
  }
}

}
}

__attribute__((weak))
void cycleAction(Key previous_key, uint8_t cycle_count) {
}

kaleidoscope::plugin::Cycle Cycle;

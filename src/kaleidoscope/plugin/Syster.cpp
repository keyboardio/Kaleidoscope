/* -*- mode: c++ -*-
 * Kaleidoscope-Syster -- Symbolic input system
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

#include <Kaleidoscope-Syster.h>
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/key_events.h"

#undef SYSTER

namespace kaleidoscope {
namespace plugin {

// --- state ---
char Syster::symbol_[SYSTER_MAX_SYMBOL_LENGTH + 1];
uint8_t Syster::symbol_pos_;
bool Syster::is_active_;

// --- helpers ---
#define isSyster(k) (k == kaleidoscope::ranges::SYSTER)

// --- api ---
void Syster::reset(void) {
  symbol_pos_ = 0;
  symbol_[0] = 0;
  is_active_ = false;
}

bool Syster::is_active(void) {
  return is_active_;
}

// --- hooks ---
EventHandlerResult Syster::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t keyState) {
  if (!is_active_) {
    if (!isSyster(mapped_key))
      return EventHandlerResult::OK;

    if (keyToggledOn(keyState)) {
      is_active_ = true;
      systerAction(StartAction, NULL);
    }
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (keyState & INJECTED)
    return EventHandlerResult::OK;

  if (isSyster(mapped_key)) {
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (mapped_key == Key_Backspace && symbol_pos_ == 0) {
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (keyToggledOff(keyState)) {
    if (mapped_key == Key_Spacebar) {
      for (uint8_t i = 0; i <= symbol_pos_; i++) {
        handleKeyswitchEvent(Key_Backspace, UnknownKeyswitchLocation, IS_PRESSED | INJECTED);
        kaleidoscope::Runtime.hid().keyboard().sendReport();
        handleKeyswitchEvent(Key_Backspace, UnknownKeyswitchLocation, WAS_PRESSED | INJECTED);
        kaleidoscope::Runtime.hid().keyboard().sendReport();
      }

      systerAction(EndAction, NULL);

      symbol_[symbol_pos_] = 0;
      systerAction(SymbolAction, symbol_);
      reset();

      return EventHandlerResult::EVENT_CONSUMED;
    }
  }

  if (keyToggledOn(keyState)) {
    if (mapped_key == Key_Backspace) {
      if (symbol_pos_ > 0)
        symbol_pos_--;
    } else {
      const char c = keyToChar(mapped_key);
      if (c)
        symbol_[symbol_pos_++] = c;
    }
  }

  return EventHandlerResult::OK;
}

}
}

__attribute__((weak)) const char keyToChar(Key key) {
  if (key.getFlags() != 0)
    return 0;

  switch (key.getKeyCode()) {
  case Key_A.getKeyCode() ... Key_Z.getKeyCode():
    return 'a' + (key.getKeyCode() - Key_A.getKeyCode());
  case Key_1.getKeyCode() ... Key_0.getKeyCode():
    return '1' + (key.getKeyCode() - Key_1.getKeyCode());
  }

  return 0;
}

__attribute__((weak)) void systerAction(kaleidoscope::plugin::Syster::action_t action, const char *symbol) {
}

kaleidoscope::plugin::Syster Syster;

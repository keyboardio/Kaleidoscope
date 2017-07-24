/* -*- mode: c++ -*-
 * Kaleidoscope-Syster -- Symbolic input system
 * Copyright (C) 2017  Gergely Nagy
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

#include <Kaleidoscope-Syster.h>

#undef SYSTER

namespace kaleidoscope {

// --- state ---
char Syster::symbol_[SYSTER_MAX_SYMBOL_LENGTH + 1];
uint8_t Syster::symbol_pos_;
bool Syster::is_active_;

// --- helpers ---
#define isSyster(k) (k == kaleidoscope::ranges::SYSTER)

// --- api ---

Syster::Syster(void) {
}

void Syster::begin(void) {
  event_handler_hook_use(eventHandlerHook);
}

void Syster::reset(void) {
  symbol_pos_ = 0;
  symbol_[0] = 0;
  is_active_ = false;
}

// --- hooks ---
Key Syster::eventHandlerHook(Key mapped_key, byte row, byte col, uint8_t key_state) {
  if (!is_active_) {
    if (!isSyster(mapped_key))
      return mapped_key;

    if (keyToggledOn(key_state)) {
      is_active_ = true;
      systerAction(StartAction, NULL);
    }
    return Key_NoKey;
  }

  if (key_state & INJECTED)
    return mapped_key;

  if (isSyster(mapped_key))
    return Key_NoKey;

  if (mapped_key == Key_Backspace && symbol_pos_ == 0)
    return Key_NoKey;

  if (keyToggledOff(key_state)) {
    if (mapped_key == Key_Spacebar) {
      for (uint8_t i = 0; i <= symbol_pos_; i++) {
        handleKeyswitchEvent(Key_Backspace, UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED | INJECTED);
        Keyboard.sendReport();
        handleKeyswitchEvent(Key_Backspace, UNKNOWN_KEYSWITCH_LOCATION, WAS_PRESSED | INJECTED);
        Keyboard.sendReport();
      }

      systerAction(EndAction, NULL);

      symbol_[symbol_pos_] = 0;
      systerAction(SymbolAction, symbol_);
      reset();

      return Key_NoKey;
    }
  }

  if (keyToggledOn(key_state)) {
    if (mapped_key == Key_Backspace) {
      if (symbol_pos_ > 0)
        symbol_pos_--;
    } else {
      const char c = keyToChar(mapped_key);
      if (c)
        symbol_[symbol_pos_++] = c;
    }
  }

  return mapped_key;
}

}

__attribute__((weak)) const char keyToChar(Key key) {
  if (key.flags != 0)
    return 0;

  switch (key.keyCode) {
  case Key_A.keyCode ... Key_Z.keyCode:
    return 'a' + (key.keyCode - Key_A.keyCode);
  case Key_1.keyCode ... Key_0.keyCode:
    return '1' + (key.keyCode - Key_1.keyCode);
  }

  return 0;
}

__attribute__((weak)) void systerAction(kaleidoscope::Syster::action_t action, const char *symbol) {
}

kaleidoscope::Syster Syster;

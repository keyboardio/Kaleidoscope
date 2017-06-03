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

using namespace KaleidoscopePlugins::Ranges;

namespace KaleidoscopePlugins {
// --- state ---
char Syster::symbol[SYSTER_MAX_SYMBOL_LENGTH + 1];
uint8_t Syster::symbolPos;
bool Syster::isActive;

// --- helpers ---

#define isSyster(k) (k == SYSTER)

// --- api ---

Syster::Syster(void) {
}

void
Syster::begin(void) {
  event_handler_hook_use(this->eventHandlerHook);
}

void
Syster::reset(void) {
  symbolPos = 0;
  symbol[0] = 0;
  isActive = false;
}

// --- hooks ---
Key
Syster::eventHandlerHook(Key mappedKey, byte row, byte col, uint8_t keyState) {
  if (!isActive) {
    if (!isSyster(mappedKey))
      return mappedKey;

    if (key_toggled_on(keyState)) {
      isActive = true;
      systerAction(StartAction, NULL);
    }
    return Key_NoKey;
  }

  if (keyState & INJECTED)
    return mappedKey;

  if (isSyster(mappedKey))
    return Key_NoKey;

  if (mappedKey == Key_Backspace && symbolPos == 0)
    return Key_NoKey;

  if (key_toggled_off(keyState)) {
    if (mappedKey == Key_Spacebar) {
      for (uint8_t i = 0; i <= symbolPos; i++) {
        handle_keyswitch_event(Key_Backspace, UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED | INJECTED);
        Keyboard.sendReport();
        handle_keyswitch_event(Key_Backspace, UNKNOWN_KEYSWITCH_LOCATION, WAS_PRESSED | INJECTED);
        Keyboard.sendReport();
      }

      systerAction(EndAction, NULL);

      symbol[symbolPos] = 0;
      systerAction(SymbolAction, symbol);
      reset();

      return Key_NoKey;
    }
  }

  if (key_toggled_on(keyState)) {
    if (mappedKey == Key_Backspace) {
      if (symbolPos > 0)
        symbolPos--;
    } else {
      const char c = keyToChar(mappedKey);
      if (c)
        symbol[symbolPos++] = c;
    }
  }

  return mappedKey;
}
};

__attribute__((weak))
const char
keyToChar(Key key) {
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

__attribute__((weak))
void
systerAction(KaleidoscopePlugins::Syster::action_t action, const char *symbol) {
}

KaleidoscopePlugins::Syster Syster;

/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Keymap-Programmer -- On-the-fly reprogrammable keymap.
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

#include <Kaleidoscope-EEPROM-Keymap-Programmer.h>

namespace KaleidoscopePlugins {
  uint16_t EEPROMKeymapProgrammer::updatePosition;
  Key EEPROMKeymapProgrammer::newKey;
  EEPROMKeymapProgrammer::state_t EEPROMKeymapProgrammer::state;

  EEPROMKeymapProgrammer::EEPROMKeymapProgrammer (void) {
  }

  void
  EEPROMKeymapProgrammer::begin (void) {
    event_handler_hook_use (eventHandlerHook);
  }

  void
  EEPROMKeymapProgrammer::nextState (void) {
    switch (state) {
    case INACTIVE:
      state = WAIT_FOR_KEY;
      break;
    case WAIT_FOR_KEY:
      state = WAIT_FOR_CODE;
      break;
    case WAIT_FOR_CODE:
      ::EEPROMKeymap.updateKey (updatePosition, newKey);
      cancel ();
      break;
    }
  }

  void
  EEPROMKeymapProgrammer::cancel (void) {
    updatePosition = 0;
    newKey = Key_NoKey;
    state = INACTIVE;
  }

  Key
  EEPROMKeymapProgrammer::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (state == INACTIVE)
      return mappedKey;

    if (state == WAIT_FOR_KEY) {
      if (key_toggled_on (keyState)) {
        updatePosition = Layer.top () * ROWS * COLS + row * COLS + col;
      }
      if (key_toggled_off (keyState)) {
        if ((uint16_t)(Layer.top () * ROWS * COLS + row * COLS + col) == updatePosition)
          nextState ();
      }
      return Key_NoKey;
    }

    // WAIT_FOR_CODE state

    if (mappedKey < KEY_1 || mappedKey > KEY_0)
      return mappedKey;

    if (!key_toggled_on (keyState))
      return Key_NoKey;

    uint8_t n;
    if (mappedKey.keyCode == Key_0.keyCode)
      n = 0;
    else
      n = mappedKey.keyCode - Key_1.keyCode + 1;

    newKey.raw = newKey.raw * 10 + n;

    return Key_NoKey;
  }

};

KaleidoscopePlugins::EEPROMKeymapProgrammer EEPROMKeymapProgrammer;

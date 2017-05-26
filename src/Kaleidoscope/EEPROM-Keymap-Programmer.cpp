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
EEPROMKeymapProgrammer::state_t EEPROMKeymapProgrammer::state;
EEPROMKeymapProgrammer::mode_t EEPROMKeymapProgrammer::mode;
Key EEPROMKeymapProgrammer::newKey;

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
        if (mode == CODE)
            state = WAIT_FOR_CODE;
        else
            state = WAIT_FOR_SOURCE_KEY;
        break;
    case WAIT_FOR_CODE:
    case WAIT_FOR_SOURCE_KEY:
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

    if (state == WAIT_FOR_SOURCE_KEY) {
        if (key_toggled_on (keyState)) {
            newKey = Layer.getKeyFromPROGMEM (Layer.top (), row, col);
        }
        if (key_toggled_off (keyState)) {
            if (newKey == Layer.getKeyFromPROGMEM (Layer.top (), row, col))
                nextState ();
        }
        return Key_NoKey;
    }

    // WAIT_FOR_CODE state

    if (mappedKey < Key_1 || mappedKey > Key_0)
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

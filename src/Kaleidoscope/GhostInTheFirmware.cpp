/* -*- mode: c++ -*-
 * Kaleidoscope-GhostInTheFirmware -- Let the keyboard write for you!
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

#include <Kaleidoscope.h>
#include <Kaleidoscope-GhostInTheFirmware.h>

namespace KaleidoscopePlugins {
GhostInTheFirmware::GhostKey *GhostInTheFirmware::ghostKeys;
bool GhostInTheFirmware::isActive;
bool GhostInTheFirmware::isPressed;
uint16_t GhostInTheFirmware::currentPos;
uint32_t GhostInTheFirmware::startTime;
uint16_t GhostInTheFirmware::pressTimeOut;
uint16_t GhostInTheFirmware::delayTimeOut;

GhostInTheFirmware::GhostInTheFirmware (void) {
}

void
GhostInTheFirmware::begin (void) {
    loop_hook_use (this->loopHook);
}

void
GhostInTheFirmware::activate (void) {
    isActive = true;
}

void
GhostInTheFirmware::configure (const GhostKey ghostKeys_[]) {
    ghostKeys = (GhostKey *)ghostKeys_;
}

void
GhostInTheFirmware::loopHook (bool postClear) {
    if (postClear || !isActive)
        return;

    if (pressTimeOut == 0) {
        pressTimeOut = pgm_read_word (&(ghostKeys[currentPos].pressTime));
        delayTimeOut = pgm_read_word (&(ghostKeys[currentPos].delay));

        if (pressTimeOut == 0) {
            currentPos = 0;
            isActive = false;
            return;
        }
        isPressed = true;
        startTime = millis ();
    } else {
        if (isPressed && ((millis () - startTime) > pressTimeOut)) {
            isPressed = false;
            startTime = millis ();

            byte row = pgm_read_byte (&(ghostKeys[currentPos].row));
            byte col = pgm_read_byte (&(ghostKeys[currentPos].col));

            handle_keyswitch_event (Key_NoKey, row, col, WAS_PRESSED);
        } else if (isPressed) {
            byte row = pgm_read_byte (&(ghostKeys[currentPos].row));
            byte col = pgm_read_byte (&(ghostKeys[currentPos].col));

            handle_keyswitch_event (Key_NoKey, row, col, IS_PRESSED);
        } else if ((millis () - startTime) > delayTimeOut) {
            currentPos++;
            pressTimeOut = 0;
        }
    }
}

};

KaleidoscopePlugins::GhostInTheFirmware GhostInTheFirmware;

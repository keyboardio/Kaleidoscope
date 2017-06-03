/* -*- mode: c++ -*-
 * Kaleidoscope-TypingBreaks -- Enforced typing breaks
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

#include <Kaleidoscope-TypingBreaks.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-Focus.h>

namespace KaleidoscopePlugins {
TypingBreaks::settings_t TypingBreaks::settings = {
    .idleTimeLimit = 10000, // 10s
    .lockTimeOut = 2700000, // 45m
    .lockLength = 300000,   //  5m
    .leftHandMaxKeys = 0,
    .rightHandMaxKeys = 0
};

uint32_t TypingBreaks::sessionStartTime;
uint32_t TypingBreaks::lastKeyTime;
uint32_t TypingBreaks::lockStartTime;
uint16_t TypingBreaks::leftHandKeys;
uint16_t TypingBreaks::rightHandKeys;
uint16_t TypingBreaks::settingsBase;

TypingBreaks::TypingBreaks (void) {
}

void
TypingBreaks::begin (void) {
    event_handler_hook_use (this->eventHandlerHook);
}

Key
TypingBreaks::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    // If we are locked, and didn't time out yet, no key has to be pressed.
    if (lockStartTime && (millis () - lockStartTime <= settings.lockLength))
        return Key_NoKey;

    // If we are locked...
    if (lockStartTime) {
        // ...and the lock has not expired yet
        if (millis () - lockStartTime <= settings.lockLength)
            return Key_NoKey; // remain locked

        // ...otherwise clear the lock
        lockStartTime = 0;
        leftHandKeys = rightHandKeys = 0;
        sessionStartTime = millis ();

        TypingBreak (false);
    }

    // Any other case, we are not locked yet! (or we just unlocked)

    // Are we still in the same session?
    if (lastKeyTime && (millis () - lastKeyTime) >= settings.idleTimeLimit) {
        // No, we are not. Clear timers and start over.
        lockStartTime = 0;
        leftHandKeys = rightHandKeys = 0;
        sessionStartTime = millis ();
    }

    // If we have a limit on the left hand, and we reached it, lock up!
    if (settings.leftHandMaxKeys && leftHandKeys >= settings.leftHandMaxKeys) {
        lockStartTime = millis ();
        TypingBreak (true);
        return Key_NoKey;
    }

    // If we have a limit on the right hand, and we reached it, lock up!
    if (settings.rightHandMaxKeys && rightHandKeys >= settings.rightHandMaxKeys) {
        lockStartTime = millis ();
        TypingBreak (true);
        return Key_NoKey;
    }

    if (settings.lockTimeOut) {
        // Is the session longer than lockTimeOut?
        if (millis () - sessionStartTime >= settings.lockTimeOut) {
            // Yeah, it is.
            lockStartTime = millis ();
            TypingBreak (true);
            return Key_NoKey;
        }
    }

    // So it seems we did not need to lock up. In this case, lets increase key
    // counters if need be.

    if (key_toggled_on (keyState)) {
        if (col <= COLS / 2)
            leftHandKeys++;
        else
            rightHandKeys++;
        lastKeyTime = millis ();
    }

    return mappedKey;
}

void
TypingBreaks::enableEEPROM (void) {
    settingsBase = ::EEPROMSettings.requestSlice (sizeof (settings));

    // If idleTime is max, assume that EEPROM is uninitialized, and store the
    // defaults.
    uint32_t idleTime;
    EEPROM.get (settingsBase, idleTime);
    if (idleTime == 0xffffffff) {
        EEPROM.put (settingsBase, settings);
    }

    EEPROM.get (settingsBase, settings);
}

bool
TypingBreaks::focusHook (const char *command) {
    enum {
        IDLE_TIME_LIMIT,
        LOCK_TIMEOUT,
        LOCK_LENGTH,
        LEFT_MAX,
        RIGHT_MAX,
    } subCommand;

    if (strncmp_P (command, PSTR ("typingbreaks."), 13) != 0)
        return false;
    if (strcmp_P (command + 13, PSTR ("idleTimeLimit")) == 0)
        subCommand = IDLE_TIME_LIMIT;
    else if (strcmp_P (command + 13, PSTR ("lockTimeOut")) == 0)
        subCommand = LOCK_TIMEOUT;
    else if (strcmp_P (command + 13, PSTR ("lockLength")) == 0)
        subCommand = LOCK_LENGTH;
    else if (strcmp_P (command + 13, PSTR ("leftMaxKeys")) == 0)
        subCommand = LEFT_MAX;
    else if (strcmp_P (command + 13, PSTR ("rightMaxKeys")) == 0)
        subCommand = RIGHT_MAX;
    else
        return false;

    switch (subCommand) {
    case IDLE_TIME_LIMIT:
        if (Serial.peek () == '\n') {
            Serial.println (settings.idleTimeLimit);
        } else {
            settings.idleTimeLimit = Serial.parseInt ();
        }
        break;
    case LOCK_TIMEOUT:
        if (Serial.peek () == '\n') {
            Serial.println (settings.lockTimeOut);
        } else {
            settings.lockTimeOut = Serial.parseInt ();
        }
        break;
    case LOCK_LENGTH:
        if (Serial.peek () == '\n') {
            Serial.println (settings.lockLength);
        } else {
            settings.lockLength = Serial.parseInt ();
        }
        break;
    case LEFT_MAX:
        if (Serial.peek () == '\n') {
            Serial.println (settings.leftHandMaxKeys);
        } else {
            settings.leftHandMaxKeys = Serial.parseInt ();
        }
        break;
    case RIGHT_MAX:
        if (Serial.peek () == '\n') {
            Serial.println (settings.rightHandMaxKeys);
        } else {
            settings.rightHandMaxKeys = Serial.parseInt ();
        }
        break;
    }

    EEPROM.put (settingsBase, settings);
    return true;
}
} // namespace KaleidoscopePlugins

KaleidoscopePlugins::TypingBreaks TypingBreaks;

__attribute__((weak))
void
TypingBreak (bool isLocked) {
}

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

namespace KaleidoscopePlugins {

  uint32_t TypingBreaks::settings::idleTimeLimit = 10000; // 10s
  uint32_t TypingBreaks::settings::lockTimeOut = 2700000; // 45m
  uint32_t TypingBreaks::settings::lockLength = 300000; // 5m
  uint16_t TypingBreaks::settings::leftHandMaxKeys;
  uint16_t TypingBreaks::settings::rightHandMaxKeys;

  uint32_t TypingBreaks::sessionStartTime;
  uint32_t TypingBreaks::lastKeyTime;
  uint32_t TypingBreaks::lockStartTime;
  uint16_t TypingBreaks::leftHandKeys;
  uint16_t TypingBreaks::rightHandKeys;

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
      return Key_NoKey;
    }

    // If we have a limit on the right hand, and we reached it, lock up!
    if (settings.rightHandMaxKeys && rightHandKeys >= settings.rightHandMaxKeys) {
      lockStartTime = millis ();
      return Key_NoKey;
    }

    if (settings.lockTimeOut) {
      // Is the session longer than lockTimeOut?
      if (millis () - sessionStartTime >= settings.lockTimeOut) {
        // Yeah, it is.
        lockStartTime = millis ();
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
};

KaleidoscopePlugins::TypingBreaks TypingBreaks;

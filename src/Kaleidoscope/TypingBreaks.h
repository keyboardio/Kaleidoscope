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

#pragma once

#include <Kaleidoscope.h>

namespace KaleidoscopePlugins {
class TypingBreaks : public KaleidoscopePlugin {
  public:
    TypingBreaks (void);

    void begin (void) final;

    static void enableEEPROM (void);
    static bool focusHook (const char *command);

    typedef struct settings_t {
        uint32_t idleTimeLimit;
        uint32_t lockTimeOut;
        uint32_t lockLength;
        uint16_t leftHandMaxKeys;
        uint16_t rightHandMaxKeys;
    } settings_t;

    static settings_t settings;

  private:
    static uint32_t sessionStartTime;
    static uint32_t lockStartTime;
    static uint32_t lastKeyTime;
    static uint16_t leftHandKeys;
    static uint16_t rightHandKeys;

    static uint16_t settingsBase;

    static Key eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState);
} 
} // namespace KaleidoscopePlugins

extern KaleidoscopePlugins::TypingBreaks TypingBreaks;

void TypingBreak (bool isLocked);

#define FOCUS_HOOK_TYPINGBREAKS FOCUS_HOOK(TypingBreaks.focusHook,        \
                                           "typingbreaks.idleTimeLimit\n" \
                                           "typingbreaks.lockTimeOut\n"   \
                                           "typingbreaks.lockLength\n"    \
                                           "typingbreaks.leftMaxKeys\n"   \
                                           "typingbreaks.rightMaxKeys")

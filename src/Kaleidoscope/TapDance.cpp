/* -*- mode: c++ -*-
 * Kaleidoscope-TapDance -- Tap-dance keys
 * Copyright (C) 2016, 2017  Gergely Nagy
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

#include <Kaleidoscope-TapDance.h>

namespace KaleidoscopePlugins {
// --- state ---
uint32_t TapDance::endTime;
uint16_t TapDance::timeOut = 200;
uint8_t TapDance::tapCount[16];
uint16_t TapDance::pressedState;
uint16_t TapDance::triggeredState;
uint16_t TapDance::releaseNextState;
Key TapDance::lastTapDanceKey;
byte TapDance::lastTapDanceRow;
byte TapDance::lastTapDanceCol;

// --- helpers ---

#define isTapDance(k) (k.raw >= Ranges::TD_FIRST && k.raw <= Ranges::TD_LAST)
#define isInSeq(k) (lastTapDanceKey.raw == k.raw)
#define stillHeld(idx) (tapCount[idx])
#define isActive() (lastTapDanceKey.raw != Key_NoKey.raw)

// --- actions ---

void
TapDance::interrupt (void) {
    uint8_t idx = lastTapDanceKey.raw - Ranges::TD_FIRST;

    tapDanceAction (idx, lastTapDanceRow, lastTapDanceCol, tapCount[idx], Interrupt);
    bitWrite (triggeredState, idx, 1);

    endTime = 0;

    if (bitRead (pressedState, idx))
        return;

    release (idx);
}

void
TapDance::timeout (void) {
    uint8_t idx = lastTapDanceKey.raw - Ranges::TD_FIRST;

    tapDanceAction (idx, lastTapDanceRow, lastTapDanceCol, tapCount[idx], Timeout);
    bitWrite (triggeredState, idx, 1);

    if (bitRead (pressedState, idx))
        return;

    lastTapDanceKey.raw = Key_NoKey.raw;

    release (idx);
}

Key
TapDance::release (uint8_t tapDanceIndex) {
    endTime = 0;
    lastTapDanceKey.raw = Key_NoKey.raw;

    bitClear (pressedState, tapDanceIndex);
    bitClear (triggeredState, tapDanceIndex);
    bitWrite (releaseNextState, tapDanceIndex, 1);
    return Key_NoKey;
}

Key
TapDance::tap (void) {
    uint8_t idx = lastTapDanceKey.raw - Ranges::TD_FIRST;

    tapCount[idx]++;
    endTime = millis () + timeOut;

    tapDanceAction (idx, lastTapDanceRow, lastTapDanceCol, tapCount[idx], Tap);

    return Key_NoKey;
}

// --- api ---

TapDance::TapDance (void) {
    lastTapDanceKey.raw = Key_NoKey.raw;
}

void
TapDance::begin (void) {
    event_handler_hook_use (this->eventHandlerHook);
    loop_hook_use (this->loopHook);
}

void
TapDance::actionKeys (uint8_t tapCount, ActionType tapDanceAction, uint8_t maxKeys, const Key tapKeys[]) {
    if (tapCount > maxKeys)
        tapCount = maxKeys;

    Key key;
    key.raw = pgm_read_word (&(tapKeys[tapCount - 1].raw));

    switch (tapDanceAction) {
    case Tap:
        break;
    case Interrupt:
    case Timeout:
        handle_keyswitch_event (key, lastTapDanceRow, lastTapDanceCol, IS_PRESSED | INJECTED);
        break;
    case Hold:
        handle_keyswitch_event (key, lastTapDanceRow, lastTapDanceCol, IS_PRESSED | WAS_PRESSED | INJECTED);
        break;
    case Release:
        Keyboard.sendReport ();
        handle_keyswitch_event (key, lastTapDanceRow, lastTapDanceCol, WAS_PRESSED | INJECTED);
        break;
    }
}

// --- hooks ---

Key
TapDance::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (keyState & INJECTED)
        return mappedKey;

    if (!key_is_pressed (keyState) && !key_was_pressed (keyState)) {
        if (isTapDance (mappedKey))
            return Key_NoKey;
        return mappedKey;
    }

    if (!isTapDance (mappedKey)) {
        if (!isActive ())
            return mappedKey;

        if (key_toggled_on (keyState))
            interrupt ();

        return mappedKey;
    }

    uint8_t tapDanceIndex = mappedKey.raw - Ranges::TD_FIRST;

    if (key_toggled_off (keyState))
        bitClear (pressedState, tapDanceIndex);

    if (!isInSeq (mappedKey)) {
        if (!isActive ()) {
            if (bitRead (triggeredState, tapDanceIndex)) {
                if (key_toggled_off (keyState))
                    return release (tapDanceIndex);
                return Key_NoKey;
            }

            lastTapDanceKey.raw = mappedKey.raw;
            lastTapDanceRow = row;
            lastTapDanceCol = col;
            return tap ();
        } else {
            if (key_toggled_off (keyState) && stillHeld (tapDanceIndex)) {
                return release (tapDanceIndex);
            }

            if (!key_toggled_on (keyState))
                return Key_NoKey;

            interrupt ();
        }
    }

    // in sequence

    if (key_toggled_off (keyState))
        return Key_NoKey;

    lastTapDanceKey.raw = mappedKey.raw;
    lastTapDanceRow = row;
    lastTapDanceCol = col;
    bitSet (pressedState, tapDanceIndex);

    if (key_toggled_on (keyState))
        return tap ();

    if (bitRead (triggeredState, tapDanceIndex))
        tapDanceAction (tapDanceIndex, row, col, tapCount[tapDanceIndex], Hold);

    return Key_NoKey;
}

void
TapDance::loopHook (bool postClear) {
    if (!postClear)
        return;

    for (uint8_t i = 0; i < 16; i++) {
        if (!bitRead (releaseNextState, i))
            continue;

        tapDanceAction (i, lastTapDanceRow, lastTapDanceCol, tapCount[i], Release);
        tapCount[i] = 0;
        bitClear (releaseNextState, i);
    }

    if (!isActive ())
        return;

    if (endTime && millis () > endTime)
        timeout();
}
} // namespace KaleidoscopePlugins

__attribute__((weak))
void
tapDanceAction (uint8_t tapDanceIndex, byte row, byte col, uint8_t tapCount, KaleidoscopePlugins::TapDance::ActionType tapDanceAction) {
}

KaleidoscopePlugins::TapDance TapDance;

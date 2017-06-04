/* -*- mode: c++ -*-
 * Kaleidoscope-TopsyTurvy -- Turn the effect of Shift upside down for certain keys
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

#include <Kaleidoscope-TopsyTurvy.h>

#define TOPSYTURVY 0b01000000

namespace KaleidoscopePlugins {

const Key *TopsyTurvy::topsyTurvyList = NULL;
uint8_t TopsyTurvy::topsyTurvyModState;

TopsyTurvy::TopsyTurvy(void) {
}

void
TopsyTurvy::begin(void) {
  event_handler_hook_use(this->eventHandlerHook);
}

void
TopsyTurvy::configure(const Key list[]) {
  topsyTurvyList = (const Key *)list;
}

Key
TopsyTurvy::eventHandlerHook(Key mappedKey, byte row, byte col, uint8_t keyState) {
  if (keyState & TOPSYTURVY)
    return mappedKey;

  if (!topsyTurvyList)
    return mappedKey;

  if (mappedKey.raw == Key_LeftShift.raw)
    bitWrite(topsyTurvyModState, 0, key_is_pressed(keyState));
  if (mappedKey.raw == Key_RightShift.raw)
    bitWrite(topsyTurvyModState, 1, key_is_pressed(keyState));

  if (!key_is_pressed(keyState) && !key_was_pressed(keyState))
    return mappedKey;

  uint8_t idx = 0;
  Key newKey;

  do {
    newKey.raw = pgm_read_word(&(topsyTurvyList[idx].raw));
    idx++;
  } while (newKey.raw != mappedKey.raw && newKey.raw != Key_NoKey.raw);

  if (newKey.raw == Key_NoKey.raw)
    return mappedKey;

  // invert the shift state

  if (!topsyTurvyModState) {
    if (key_is_pressed(keyState))
      Keyboard.press(Key_LeftShift.keyCode);
    handle_keyswitch_event(mappedKey, row, col, keyState | TOPSYTURVY);
    Keyboard.sendReport();
    if (key_toggled_off(keyState))
      Keyboard.release(Key_LeftShift.keyCode);
  } else {
    Keyboard.release(Key_LeftShift.keyCode);
    Keyboard.release(Key_RightShift.keyCode);
    Keyboard.sendReport();
    handle_keyswitch_event(mappedKey, row, col, keyState | TOPSYTURVY);
    Keyboard.sendReport();

    if (bitRead(topsyTurvyModState, 0))
      Keyboard.press(Key_LeftShift.keyCode);
    if (bitRead(topsyTurvyModState, 1))
      Keyboard.press(Key_RightShift.keyCode);
  }

  return Key_NoKey;
}

}  // namespace KaleidoscopePlugins

KaleidoscopePlugins::TopsyTurvy TopsyTurvy;

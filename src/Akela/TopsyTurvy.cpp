/* -*- mode: c++ -*-
 * Akela -- Animated Keyboardio Extension Library for Anything
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

#include <Akela-TopsyTurvy.h>

#define TOPSYTURVY 0b01000000

namespace Akela {

  const Key *TopsyTurvy::topsyTurvyList = NULL;
  uint8_t TopsyTurvy::topsyTurvyModState;

  TopsyTurvy::TopsyTurvy (void) {
  }

  void
  TopsyTurvy::begin (void) {
    event_handler_hook_add (this->eventHandlerHook);
    loop_hook_add (this->loopHook);
  }

  void
  TopsyTurvy::configure (const Key list[]) {
    topsyTurvyList = (const Key *)list;
  }

  void
  TopsyTurvy::on (void) {
    event_handler_hook_replace (this->noOpHook, this->eventHandlerHook);
    loop_hook_replace (this->noOpLoopHook, this->loopHook);
  }

  void
  TopsyTurvy::off (void) {
    event_handler_hook_replace (this->eventHandlerHook, this->noOpHook);
    loop_hook_replace (this->loopHook, this->noOpLoopHook);
  }

  Key
  TopsyTurvy::noOpHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    return mappedKey;
  }

  void
  TopsyTurvy::noOpLoopHook (bool postClear) {
  }

  void
  TopsyTurvy::loopHook (bool postClear) {
    if (postClear)
      return;

    bitWrite (topsyTurvyModState, 0, Keyboard.isModifierActive (Key_LShift.keyCode));
    bitWrite (topsyTurvyModState, 1, Keyboard.isModifierActive (Key_RShift.keyCode));
  }

  Key
  TopsyTurvy::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (keyState & TOPSYTURVY)
      return mappedKey;

    if (!topsyTurvyList)
      return mappedKey;

    if (!key_is_pressed (keyState) && !key_was_pressed (keyState))
      return mappedKey;

    uint8_t idx = 0;
    Key newKey;

    do {
      newKey.raw = pgm_read_word (&(topsyTurvyList[idx].raw));
      idx++;
    } while (newKey.raw != mappedKey.raw && newKey.raw != Key_NoKey.raw);

    if (newKey.raw == Key_NoKey.raw)
      return mappedKey;

    // invert the shift state

    if (!topsyTurvyModState) {
      Keyboard.press (Key_LShift.keyCode);
      handle_key_event (mappedKey, row, col, keyState | TOPSYTURVY);
      Keyboard.sendReport ();
      Keyboard.release (Key_LShift.keyCode);
    } else {
      Keyboard.release (Key_LShift.keyCode);
      Keyboard.release (Key_RShift.keyCode);
      handle_key_event (mappedKey, row, col, keyState | TOPSYTURVY);
      Keyboard.sendReport ();

      if (bitRead (topsyTurvyModState, 0))
        Keyboard.press (Key_LShift.keyCode);
      if (bitRead (topsyTurvyModState, 1))
        Keyboard.press (Key_RShift.keyCode);
    }

    return Key_NoKey;
  }

};

Akela::TopsyTurvy TopsyTurvy;

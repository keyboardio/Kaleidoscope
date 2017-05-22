/* -*- mode: c++ -*-
 * Kaleidoscope-ShapeShifter -- Change the shifted symbols on any key of your choice
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

#include <Kaleidoscope-ShapeShifter.h>

namespace KaleidoscopePlugins {

  const ShapeShifter::dictionary_t *ShapeShifter::dictionary = NULL;
  bool ShapeShifter::modActive;

  ShapeShifter::ShapeShifter (void) {
  }

  void
  ShapeShifter::begin (void) {
    event_handler_hook_use (this->eventHandlerHook);
    loop_hook_use (this->loopHook);
  }

  void
  ShapeShifter::configure (const dictionary_t dictionary_[]) {
    dictionary = (const dictionary_t *)dictionary_;
  }

  void
  ShapeShifter::loopHook (bool postClear) {
    if (postClear)
      return;

    modActive = Keyboard.isModifierActive (Key_LeftShift.keyCode) ||
      Keyboard.isModifierActive (Key_RightShift.keyCode);
  }

  Key
  ShapeShifter::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (!dictionary)
      return mappedKey;

    // If Shift is not active, bail out early.
    if (!modActive)
      return mappedKey;

    Key orig, repl;

    // Try to find the current key in the dictionary
    uint8_t i = 0;
    do {
      orig.raw = pgm_read_word (&(dictionary[i].original.raw));
      i++;
    } while (orig.raw != Key_NoKey.raw &&
             orig.raw != mappedKey.raw);
    i--;

    // If not found, bail out.
    if (orig.raw == Key_NoKey.raw)
      return mappedKey;

    repl.raw = pgm_read_word (&(dictionary[i].replacement.raw));

    // If found, handle the alternate key instead
    return repl;
  }

};

KaleidoscopePlugins::ShapeShifter ShapeShifter;

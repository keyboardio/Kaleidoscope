/* -*- mode: c++ -*-
 * Akela -- Animated Keyboardio Extension Library for Anything
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

#include <Akela-ShapeShifter.h>

namespace Akela {

  const ShapeShifter::dictionary_t *ShapeShifter::dictionary = NULL;
  bool ShapeShifter::modActive;

  ShapeShifter::ShapeShifter (void) {
  }

  void
  ShapeShifter::begin (void) {
    event_handler_hook_add (this->eventHandlerHook);
    loop_hook_add (this->loopHook);
  }

  void
  ShapeShifter::configure (const dictionary_t dictionary_[]) {
    dictionary = (const dictionary_t *)dictionary_;
  }

  void
  ShapeShifter::on (void) {
    event_handler_hook_replace (this->noOpHook, this->eventHandlerHook);
    loop_hook_replace (this->noOpLoopHook, this->loopHook);
  }

  void
  ShapeShifter::off (void) {
    event_handler_hook_replace (this->eventHandlerHook, this->noOpHook);
    loop_hook_replace (this->loopHook, this->noOpLoopHook);
  }

  Key
  ShapeShifter::noOpHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    return mappedKey;
  }

  void
  ShapeShifter::noOpLoopHook (bool postClear) {
  }

  void
  ShapeShifter::loopHook (bool postClear) {
    if (postClear)
      return;

    modActive = Keyboard.isModifierActive (Key_LShift.keyCode) ||
      Keyboard.isModifierActive (Key_RShift.keyCode);
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

Akela::ShapeShifter ShapeShifter;

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

#include <Akela-SpaceCadet.h>

namespace Akela {

  uint8_t SpaceCadetShift::parenNeeded;
  uint32_t SpaceCadetShift::startTime;
  uint16_t SpaceCadetShift::timeOut = 1000;
  Key SpaceCadetShift::leftParen, SpaceCadetShift::rightParen;

  SpaceCadetShift::SpaceCadetShift () {
    leftParen.raw = Key_9.raw;
    rightParen.raw = Key_0.raw;
  }

  void
  SpaceCadetShift::begin () {
    event_handler_hook_use (this->eventHandlerHook);
  }

  void
  SpaceCadetShift::configure (Key left, Key right) {
    leftParen = left;
    rightParen = right;
  }

  void
  SpaceCadetShift::on (void) {
    event_handler_hook_replace (this->noOpHook, this->eventHandlerHook);
  }

  void
  SpaceCadetShift::off (void) {
    event_handler_hook_replace (this->eventHandlerHook, this->noOpHook);
  }

  Key
  SpaceCadetShift::noOpHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    return mappedKey;
  }

  Key
  SpaceCadetShift::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    // If nothing happened, bail out fast.
    if (!key_is_pressed (keyState) && !key_was_pressed (keyState)) {
      return mappedKey;
    }

    // If a key has been just toggled on...
    if (key_toggled_on (keyState)) {
      if (mappedKey.raw == Key_LShift.raw) { // if it is LShift, remember it
        bitWrite (parenNeeded, 0, 1);
        startTime = millis ();
      } else if (mappedKey.raw == Key_RShift.raw) { // if it is RShift, remember it
        bitWrite (parenNeeded, 1, 1);
        startTime = millis ();
      } else { // if it is something else, we do not need a paren at the end.
        parenNeeded = 0;
        startTime = 0;
      }

      // this is all we need to do on keypress, let the next handler do its thing too.
      return mappedKey;
    }

    // if the state is empty, that means that either the shifts weren't pressed,
    // or we used another key in the interim. in both cases, nothing special to do.
    if (!parenNeeded)
      return mappedKey;

    // if we timed out, that means we need to keep pressing shift, but won't
    // need the parens in the end.
    if ((millis () - startTime) >= timeOut) {
      parenNeeded = 0;
      return mappedKey;
    }

    // if we have a state, but the key in question is not either of the shifts,
    // return. This can happen when another key is released, and that should not
    // interrupt us.
    if (mappedKey.raw != Key_LShift.raw &&
        mappedKey.raw != Key_RShift.raw)
      return mappedKey;

    // if a key toggled off (and that must be one of the shifts at this point),
    // send the parens too (if we were interrupted, we bailed out earlier).
    if (key_toggled_off (keyState)) {
      Key paren = leftParen;
      if (bitRead (parenNeeded, 1))
        paren = rightParen;

      handle_key_event (mappedKey, row, col, IS_PRESSED | INJECTED);
      handle_key_event (paren, row, col, IS_PRESSED | INJECTED);
      Keyboard.sendReport ();

      parenNeeded = 0;
    }

    return mappedKey;
  }

};

Akela::SpaceCadetShift SpaceCadetShift;

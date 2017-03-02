/* -*- mode: c++ -*-
 * Kaleidoscope-LED-AlphaSquare -- 4x4 pixel LED alphabet
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

#include <Kaleidoscope-LED-AlphaSquare.h>

namespace KaleidoscopePlugins {
  namespace LEDEffects {
    uint16_t AlphaSquareEffect::length = 1000;
    uint32_t AlphaSquareEffect::endTimeLeft, AlphaSquareEffect::endTimeRight;
    Key AlphaSquareEffect::lastKeyLeft, AlphaSquareEffect::lastKeyRight;

    AlphaSquareEffect::AlphaSquareEffect (void) {
    }

    void
    AlphaSquareEffect::begin (void) {
      Kaleidoscope.useEventHandlerHook (eventHandlerHook);
      LEDMode::begin ();
    }

    void
    AlphaSquareEffect::update (void) {
      if (endTimeLeft && millis () > endTimeLeft) {
        ::AlphaSquare.clear (lastKeyLeft);
        endTimeLeft = 0;
      }
      if (endTimeRight && millis () > endTimeRight) {
        ::AlphaSquare.clear (lastKeyRight, 10);
        endTimeRight = 0;
      }
    }

    Key
    AlphaSquareEffect::eventHandlerHook (Key key, byte row, byte col, uint8_t keyState) {
      if (keyState & INJECTED)
        return key;

      if (key < Key_A || key > Key_0)
        return key;

      if (!key_is_pressed (keyState))
        return key;

      uint8_t displayCol = 2;
      Key prevKey = lastKeyLeft;

      if (col < COLS / 2) {
        lastKeyLeft = key;
        endTimeLeft = millis () + length;
      } else {
        prevKey = lastKeyRight;
        lastKeyRight = key;
        endTimeRight = millis () + length;
        displayCol = 10;
      }

      if (prevKey != key)
        ::AlphaSquare.clear (prevKey, displayCol);
      ::AlphaSquare.display (key, displayCol);
      return key;
    }
  };
};

KaleidoscopePlugins::LEDEffects::AlphaSquareEffect AlphaSquareEffect;

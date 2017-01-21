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

#include <Akela-Core.h>

namespace Akela {
  namespace LEDEffects {
    class StalkerEffect : public KeyboardioPlugin {
    public:
      StalkerEffect (void);

      static void configure (const cRGB highlightColor);
      virtual void begin (void) final;

    private:
      static cRGB highlightColor;
      static uint8_t map[ROWS][COLS];

      static Key eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState);
      static void loopHook (bool postClear);
    };
  };
};

extern Akela::LEDEffects::StalkerEffect StalkerEffect;

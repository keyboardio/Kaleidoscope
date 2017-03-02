/* -*- mode: c++ -*-
 * Kaleidoscope-LED-Stalker -- Stalk keys pressed by lighting up and fading back the LED under them
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

#include <Kaleidoscope.h>
#include <Kaleidoscope-LEDControl.h>

#define STALKER(n, ...) (({static KaleidoscopePlugins::LEDEffects::Stalker::n _effect (__VA_ARGS__); &_effect;}))

namespace KaleidoscopePlugins {
  namespace LEDEffects {
    class StalkerEffect : public LEDMode {
    public:
      class ColorComputer {
      public:
        virtual cRGB compute (uint8_t step) = 0;
      };

      StalkerEffect (void);

      virtual void begin (void) final;
      virtual void update (void) final;

      static void configure (ColorComputer *colorComputer);
      static uint16_t stepLength;

    private:
      static uint32_t stepEndTime;
      static ColorComputer *colorComputer;
      static uint8_t map[ROWS][COLS];

      static Key eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState);
    };

    namespace Stalker {

      class Haunt : public StalkerEffect::ColorComputer {
      public:
        Haunt (const cRGB highlightColor);
        Haunt (void) : Haunt ({0x40, 0x80, 0x80}) {};
        Haunt (void *) : Haunt () {};

        virtual cRGB compute (uint8_t step) final;

      private:
        static float mr, mg, mb;
      };

      class BlazingTrail : public StalkerEffect::ColorComputer {
      public:
        BlazingTrail (...);

        virtual cRGB compute (uint8_t step) final;
      };

    };

  };
};

extern KaleidoscopePlugins::LEDEffects::StalkerEffect StalkerEffect;

/* -*- mode: c++ -*-
 * Kaleidoscope-Cycle -- Key sequence cycling dead key for Kaleidoscope.
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

#pragma once

#include <Kaleidoscope-Ranges.h>

#define Key_Cycle (Key){ .raw = KaleidoscopePlugins::Ranges::CYCLE }

#define cycleThrough(...) ({                                \
      static const Key __k[] PROGMEM = { __VA_ARGS__ };     \
      Cycle.replace (sizeof (__k) / sizeof (Key), &__k[0]); \
    })

namespace KaleidoscopePlugins {
  class Cycle : public KaleidoscopePlugin {
  public:
    Cycle (void);

    virtual void begin (void) final;

    static void replace (Key key);
    static void replace (uint8_t cycleSize, const Key cycleSteps[]);

  private:
    static Key lastNonCycleKey;
    static uint8_t cycleCount;

    static Key eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState);
  };
};

void cycleAction (Key previousKey, uint8_t cycleCount);

extern KaleidoscopePlugins::Cycle Cycle;

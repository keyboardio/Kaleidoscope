/* -*- mode: c++ -*-
 * Kaleidoscope-Syster -- Symbolic input system
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

#pragma once

#include <Kaleidoscope.h>
#include <Kaleidoscope-Ranges.h>

#define SYSTER_MAX_SYMBOL_LENGTH 32

#define SYSTER (Key){ .raw = KaleidoscopePlugins::Ranges::SYSTER }

namespace KaleidoscopePlugins {
class Syster : public KaleidoscopePlugin {
  public:
    typedef enum {
        StartAction,
        EndAction,
        SymbolAction
    } action_t;

    Syster (void);

    virtual void begin (void) final;
    static void reset (void);

  private:
    static char symbol[SYSTER_MAX_SYMBOL_LENGTH + 1];
    static uint8_t symbolPos;
    static bool isActive;

    static Key eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState);
};
};

const char keyToChar (Key key);
void systerAction (KaleidoscopePlugins::Syster::action_t action, const char *symbol);

extern KaleidoscopePlugins::Syster Syster;

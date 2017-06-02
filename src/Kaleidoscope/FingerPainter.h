/* -*- mode: c++ -*-
 * Kaleidoscope-FingerPainter -- On-the-fly keyboard painting.
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

#include <Kaleidoscope-LEDControl.h>

namespace KaleidoscopePlugins {
class FingerPainter : public LEDMode {
  public:
    FingerPainter (void);

    virtual void begin (void) final;
    virtual void update (void) final;

    static void toggleEdit (void);
    static bool focusHook (const char *command);

  private:
    static uint16_t colorBase;
    static bool editMode;

    static Key eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState);
};
};

extern KaleidoscopePlugins::FingerPainter FingerPainter;

#define FOCUS_HOOK_FINGERPAINTER FOCUS_HOOK(FingerPainter.focusHook,  \
                                            "fingerpainter.toggle\n"  \
                                            "fingerpainter.clear")

/* -*- mode: c++ -*-
 * Kaleidoscope-LED-Palette-Theme -- Palette-based LED theme foundation
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
#include <Kaleidoscope-LEDControl.h>

namespace KaleidoscopePlugins {
class LEDPaletteTheme : public KaleidoscopePlugin {
  public:
    LEDPaletteTheme (void);

    virtual void begin (void) final;

    static uint16_t reserveThemes (uint8_t maxThemes);
    static void update (uint16_t themeBase, uint8_t theme);

    static bool paletteFocusHook (const char *command);
    static bool themeFocusHandler (const char *command, const char *expectedCommand,
                                   uint16_t themeBase, uint8_t maxThemes);

    static uint8_t transparentIndex;

  private:
    static uint16_t paletteBase;

    static const bool lookupColor (uint16_t mapBase, uint16_t loc, cRGB *color);
};
};

extern KaleidoscopePlugins::LEDPaletteTheme LEDPaletteTheme;

#define FOCUS_HOOK_LEDPALETTETHEME                                      \
  FOCUS_HOOK(LEDPaletteTheme.paletteFocusHook,                          \
             "palette")

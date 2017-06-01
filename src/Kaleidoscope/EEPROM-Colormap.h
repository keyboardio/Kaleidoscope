/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Colormap -- Per-layer colormap effect
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
#include <Kaleidoscope-LED-Palette-Theme.h>

namespace KaleidoscopePlugins {
class EEPROMColormapEffect : public LEDMode {
  public:
    EEPROMColormapEffect (void);

    virtual void update (void) final;
    void configure (uint8_t maxLayers);

    static bool focusHook (const char *command);

  private:
    static uint8_t maxLayers;
    static uint16_t mapBase;
};
};

extern KaleidoscopePlugins::EEPROMColormapEffect EEPROMColormapEffect;

#define FOCUS_HOOK_COLORMAP FOCUS_HOOK(EEPROMColormapEffect.focusHook,  \
                                       "colormap.map")

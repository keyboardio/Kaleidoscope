/* -*- mode: c++ -*-
 * Kaleidoscope-Colormap -- Per-layer colormap effect
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

#include <Kaleidoscope-Colormap.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-Focus.h>
#include <EEPROM.h>

namespace KaleidoscopePlugins {

uint16_t ColormapEffect::mapBase;
uint8_t ColormapEffect::maxLayers;

ColormapEffect::ColormapEffect (void) {
}

void
ColormapEffect::configure (uint8_t maxLayers_) {
    USE_PLUGINS (&::EEPROMSettings, &::LEDPaletteTheme);

    maxLayers = maxLayers_;
    mapBase = ::LEDPaletteTheme.reserveThemes (maxLayers);
}

void
ColormapEffect::update (void) {
    for (uint8_t l = 0; l < 32; l++) {
        if (!Layer.isOn (l))
            continue;

        ::LEDPaletteTheme.update (mapBase, l);
    }
}

bool
ColormapEffect::focusHook (const char *command) {
    return ::LEDPaletteTheme.themeFocusHandler (command, PSTR("colormap.map"), mapBase, maxLayers);
}

};

KaleidoscopePlugins::ColormapEffect ColormapEffect;

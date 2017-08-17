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

#include <EEPROM.h>

#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-Focus.h>

namespace kaleidoscope {

uint16_t ColormapEffect::map_base_;
uint8_t ColormapEffect::max_layers_;
uint8_t ColormapEffect::last_highest_layer_;

void ColormapEffect::setup(void) {
  Kaleidoscope.use(&::EEPROMSettings, &::LEDPaletteTheme);
}

void ColormapEffect::max_layers(uint8_t max_) {
  if (map_base_ != 0)
    return;

  max_layers_ = max_;
  map_base_ = ::LEDPaletteTheme.reserveThemes(max_layers_);
}

void ColormapEffect::onActivate(void) {
  last_highest_layer_ = Layer.top();
  ::LEDPaletteTheme.updateHandler(map_base_, last_highest_layer_);
}

void ColormapEffect::update(void) {
  if (Layer.top() == last_highest_layer_)
    return;

  onActivate();
}

void ColormapEffect::refreshAt(byte row, byte col) {
  ::LEDPaletteTheme.refreshAt(map_base_, last_highest_layer_, row, col);
}

bool ColormapEffect::focusHook(const char *command) {
  return ::LEDPaletteTheme.themeFocusHandler(command, PSTR("colormap.map"),
         map_base_, max_layers_);
}

}

kaleidoscope::ColormapEffect ColormapEffect;

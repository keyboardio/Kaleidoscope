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

#pragma once

#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LED-Palette-Theme.h>

namespace kaleidoscope {
class ColormapEffect : public LEDMode {
 public:
  ColormapEffect(void) {}

  void max_layers(uint8_t max_);

  static bool focusHook(const char *command);
  static bool focusHookLayerwise(const char *command);

 protected:
  void setup(void) final;
  void onActivate(void) final;
  void update(void) final;
  void refreshAt(byte row, byte col) final;

 private:
  static uint8_t last_highest_layer_;
  static uint8_t max_layers_;
  static uint16_t map_base_;
  static bool dirty_;
};
}

extern kaleidoscope::ColormapEffect ColormapEffect;

#define FOCUS_HOOK_COLORMAP FOCUS_HOOK(ColormapEffect.focusHook,  \
                                       "colormap.map")

#define FOCUS_HOOK_COLORMAP_LAYER FOCUS_HOOK(ColormapEffect.focusHookLayerwise, \
                                             "colormap.layer")

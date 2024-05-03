/* -*- mode: c++ -*-
 * Kaleidoscope-Colormap -- Per-layer colormap effect
 * Copyright (C) 2022  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify it under it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <Arduino.h>  // for PROGMEM

#include "kaleidoscope/device/device.h"             // for cRGB, CRGB
#include "kaleidoscope/plugin.h"                    // for Plugin
#include "kaleidoscope/plugin/LED-Palette-Theme.h"  // for LEDPaletteTheme

namespace kaleidoscope {
namespace plugin {

namespace ledpalette {
extern bool palette_defined;
extern const cRGB palette[];

template<uint8_t _color_count>
void configurePalette(cRGB const (&colors)[_color_count]) {
  for (uint8_t i = 0; i < LEDPaletteTheme::getPaletteSize(); i++) {
    if (i < _color_count) {
      palette[i] = colors[i];
    } else {
      palette[i] = CRGB(0x00, 0x00, 0x00);
    }
  }
  bool palette_defined = true;
}
}  // namespace ledpalette

class DefaultPalette : public Plugin {
 public:
  static void setup();
};

}  // namespace plugin
}  // namespace kaleidoscope

// clang-format off

#define PALETTE(p00, p01, p02, p03, p04, p05, p06, p07,   \
                p08, p09, p0a, p0b, p0c, p0d, p0e, ...)   \
  namespace kaleidoscope {                                \
  namespace plugin {                                      \
  namespace ledpalette {                                  \
    const cRGB palette[] PROGMEM = {                      \
      p00, p01, p02, p03, p04, p05, p06, p07,             \
      p08, p09, p0a, p0b, p0c, p0d, p0e, ##__VA_ARGS__    \
    };                                                    \
    bool palette_defined = true;                          \
  } /* defaultcolormap */                                 \
  } /* plugin */                                          \
  } /* kaleidoscope */

// clang-format on

extern kaleidoscope::plugin::DefaultPalette DefaultPalette;

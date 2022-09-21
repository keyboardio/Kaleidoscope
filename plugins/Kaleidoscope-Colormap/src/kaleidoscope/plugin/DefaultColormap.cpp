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

#include "kaleidoscope/plugin/Colormap.h"  // for Colormap
#include "kaleidoscope/plugin/DefaultColormap.h"

#include <Arduino.h>                         // for F, PSTR, __FlashStringHelper
#include <Kaleidoscope-FocusSerial.h>        // for Focus, FocusSerial
#include <Kaleidoscope-LEDControl.h>         // for LEDControl
#include <Kaleidoscope-LED-Palette-Theme.h>  // for LEDPaletteTheme
#include <stdint.h>                          // for uint8_t, uint16_t

#include "kaleidoscope/KeyAddr.h"  // for KeyAddr
#include "kaleidoscope/Runtime.h"  // for Runtime, Runtime_

namespace kaleidoscope {
namespace plugin {

namespace defaultcolormap {
__attribute__((weak)) extern const cRGB palette[]                                                                                                = {};
__attribute__((weak)) extern bool palette_defined                                                                                                = false;
__attribute__((weak)) extern const uint8_t colormaps[][kaleidoscope_internal::device.matrix_rows * kaleidoscope_internal::device.matrix_columns] = {};
__attribute__((weak)) extern uint8_t colormap_layers                                                                                             = 0;
}  // namespace defaultcolormap

void DefaultColormap::setup() {
  // If the colormap is already initialized, return early.
  if (!::ColormapEffect.isUninitialized())
    return;

  install();
}

void DefaultColormap::install() {
  if (!defaultcolormap::palette_defined) return;

  for (uint8_t i = 0; i < 16; i++) {
    cRGB color;

    color.r = pgm_read_byte(&(defaultcolormap::palette[i].r));
    color.g = pgm_read_byte(&(defaultcolormap::palette[i].g));
    color.b = pgm_read_byte(&(defaultcolormap::palette[i].b));

    ::LEDPaletteTheme.updatePaletteColor(i, color);
  }

  if (defaultcolormap::colormap_layers == 0) return;

  for (uint8_t layer = 0; layer < defaultcolormap::colormap_layers; layer++) {
    for (int8_t i = 0; i < Runtime.device().numKeys(); i++) {
      const int8_t post = Runtime.device().ledDriver().getLedIndex(i);
      const uint8_t idx = pgm_read_byte(&(defaultcolormap::colormaps[layer][i]));
      ::ColormapEffect.updateColorIndexAtPosition(layer, post, idx);
    }
  }
  Runtime.storage().commit();

  ::LEDControl.refreshAll();
}

EventHandlerResult DefaultColormap::onFocusEvent(const char *input) {
  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  const char *cmd = PSTR("colormap.install");

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd);

  if (!::Focus.inputMatchesCommand(input, cmd))
    return EventHandlerResult::OK;

  install();
  return EventHandlerResult::EVENT_CONSUMED;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::DefaultColormap DefaultColormap;

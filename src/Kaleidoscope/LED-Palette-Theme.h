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

namespace kaleidoscope {

class LEDPaletteTheme : public KaleidoscopePlugin {
 public:
  LEDPaletteTheme(void) {}

  void begin(void) final;

  static uint16_t reserveThemes(uint8_t max_themes);
  static void updateHandler(uint16_t theme_base, uint8_t theme);
  static void refreshAt(uint16_t theme_base, uint8_t theme, byte row, byte col);

  static const uint8_t lookupColorIndexAtPosition(uint16_t theme_base, uint16_t position);
  static const cRGB lookupColorAtPosition(uint16_t theme_base, uint16_t position);
  static void updateColorIndexAtPosition(uint16_t theme_base, uint16_t position, uint8_t color_index);

  static const cRGB lookupPaletteColor(uint8_t palette_index);

  static bool paletteFocusHook(const char *command);
  static bool themeFocusHandler(const char *command, const char *expected_command,
                                uint16_t theme_base, uint8_t max_themes);

  static bool themeLayerFocusHandler(const char *command,
                                     const char *expected_command,
                                     uint16_t theme_base,
                                     uint8_t max_themes);
 private:
  static uint16_t palette_base_;
};

}

extern kaleidoscope::LEDPaletteTheme LEDPaletteTheme;

#define FOCUS_HOOK_LEDPALETTETHEME                                      \
  FOCUS_HOOK(LEDPaletteTheme.paletteFocusHook,                          \
             "palette")

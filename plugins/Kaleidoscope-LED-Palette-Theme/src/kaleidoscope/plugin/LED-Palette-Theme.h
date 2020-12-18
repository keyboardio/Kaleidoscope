/* -*- mode: c++ -*-
 * Kaleidoscope-LED-Palette-Theme -- Palette-based LED theme foundation
 * Copyright (C) 2017, 2018  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "kaleidoscope/Runtime.h"
#include <Kaleidoscope-LEDControl.h>

namespace kaleidoscope {
namespace plugin {

class LEDPaletteTheme : public kaleidoscope::Plugin {
 public:
  LEDPaletteTheme(void) {}

  static uint16_t reserveThemes(uint8_t max_themes);
  static void updateHandler(uint16_t theme_base, uint8_t theme);
  static void refreshAt(uint16_t theme_base, uint8_t theme, KeyAddr key_addr);

  static const uint8_t lookupColorIndexAtPosition(uint16_t theme_base, uint16_t position);
  static const cRGB lookupColorAtPosition(uint16_t theme_base, uint16_t position);
  static void updateColorIndexAtPosition(uint16_t theme_base, uint16_t position, uint8_t color_index);

  static const cRGB lookupPaletteColor(uint8_t palette_index);

  EventHandlerResult onFocusEvent(const char *command);
  EventHandlerResult themeFocusEvent(const char *command,
                                     const char *expected_command,
                                     uint16_t theme_base, uint8_t max_themes);

 private:
  static uint16_t palette_base_;
};

}
}

extern kaleidoscope::plugin::LEDPaletteTheme LEDPaletteTheme;

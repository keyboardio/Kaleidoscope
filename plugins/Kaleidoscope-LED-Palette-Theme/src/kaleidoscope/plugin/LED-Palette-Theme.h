/* -*- mode: c++ -*-
 * Kaleidoscope-LED-Palette-Theme -- Palette-based LED theme foundation
 * Copyright (C) 2017-2022  Keyboard.io, Inc
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

#include <stdint.h>  // for uint16_t, uint8_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/device/device.h"         // for cRGB
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin

namespace kaleidoscope {
namespace plugin {

class LEDPaletteTheme : public kaleidoscope::Plugin {
 public:
  static uint16_t reserveThemes(uint8_t max_themes);
  static void updateHandler(uint16_t theme_base, uint8_t theme);
  static void refreshAt(uint16_t theme_base, uint8_t theme, KeyAddr key_addr);

  static const uint8_t lookupColorIndexAtPosition(uint16_t theme_base, uint16_t position);
  static const cRGB lookupColorAtPosition(uint16_t theme_base, uint16_t position);
  static void updateColorIndexAtPosition(uint16_t theme_base, uint16_t position, uint8_t color_index);

  static void updatePaletteColor(uint8_t palette_index, cRGB color);
  static const cRGB lookupPaletteColor(uint8_t palette_index);

  EventHandlerResult onFocusEvent(const char *input);
  EventHandlerResult themeFocusEvent(const char *input,
                                     const char *expected_input,
                                     uint16_t theme_base,
                                     uint8_t max_themes);
  static bool isThemeUninitialized(uint16_t theme_base, uint8_t max_themes);

 private:
  static uint16_t palette_base_;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::LEDPaletteTheme LEDPaletteTheme;

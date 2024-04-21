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

#include "kaleidoscope/plugin/LED-Palette-Theme.h"

#include <Arduino.h>                       // for PSTR
#include <Kaleidoscope-EEPROM-Settings.h>  // for EEPROMSettings
#include <Kaleidoscope-FocusSerial.h>      // for Focus, FocusSerial
#include <stdint.h>                        // for uint8_t, uint16_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for cRGB, VirtualProps::Storage, Device, Base...
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/plugin/LEDControl.h"     // for LEDControl

namespace kaleidoscope {
namespace plugin {

uint16_t LEDPaletteTheme::palette_base_;
uint8_t LEDPaletteTheme::palette_size_ = 24;

uint16_t LEDPaletteTheme::reserveThemes(uint8_t max_themes) {
  if (!palette_base_)
    palette_base_ = ::EEPROMSettings.requestSlice(palette_size_ * sizeof(cRGB));

  return ::EEPROMSettings.requestSlice(max_themes * Runtime.device().led_count / 2);
}

void LEDPaletteTheme::updateHandler(uint16_t theme_base, uint8_t theme) {
  if (!Runtime.has_leds)
    return;

  uint16_t map_base = theme_base + (theme * Runtime.device().led_count / 2);

  for (uint8_t pos = 0; pos < Runtime.device().led_count; pos++) {
    cRGB color = lookupColorAtPosition(map_base, pos);
    ::LEDControl.setCrgbAt(pos, color);
  }
}

void LEDPaletteTheme::refreshAt(uint16_t theme_base, uint8_t theme, KeyAddr key_addr) {
  if (!Runtime.has_leds)
    return;

  uint16_t map_base = theme_base + (theme * Runtime.device().led_count / 2);
  uint8_t pos       = Runtime.device().getLedIndex(key_addr);

  cRGB color = lookupColorAtPosition(map_base, pos);
  ::LEDControl.setCrgbAt(key_addr, color);
}


const uint8_t LEDPaletteTheme::lookupColorIndexAtPosition(uint16_t map_base, uint16_t position) {
  uint8_t color_index;

  color_index = Runtime.storage().read(map_base + position / 2);
  if (position % 2)
    color_index &= ~0xf0;
  else
    color_index >>= 4;

  return color_index;
}

const cRGB LEDPaletteTheme::lookupColorAtPosition(uint16_t map_base, uint16_t position) {
  uint8_t color_index = lookupColorIndexAtPosition(map_base, position);

  return lookupPaletteColor(color_index);
}

const cRGB LEDPaletteTheme::lookupPaletteColor(uint8_t color_index) {
  cRGB color;

  Runtime.storage().get(palette_base_ + color_index * sizeof(cRGB), color);
  color.r ^= 0xff;
  color.g ^= 0xff;
  color.b ^= 0xff;

  return color;
}

void LEDPaletteTheme::updateColorIndexAtPosition(uint16_t map_base, uint16_t position, uint8_t color_index) {
  uint8_t indexes;

  indexes = Runtime.storage().read(map_base + position / 2);
  if (position % 2) {
    uint8_t other = indexes >> 4;
    indexes       = (other << 4) + color_index;
  } else {
    uint8_t other = indexes & ~0xf0;
    indexes       = (color_index << 4) + other;
  }
  Runtime.storage().update(map_base + position / 2, indexes);
}

void LEDPaletteTheme::updatePaletteColor(uint8_t palette_index, cRGB color) {
  color.r ^= 0xff;
  color.g ^= 0xff;
  color.b ^= 0xff;

  Runtime.storage().put(palette_base_ + palette_index * sizeof(color), color);
}

bool LEDPaletteTheme::isThemeUninitialized(uint16_t theme_base, uint8_t max_themes) {
  bool paletteEmpty = Runtime.storage().isSliceUninitialized(palette_base_, palette_size_ * sizeof(cRGB));
  bool themeEmpty   = Runtime.storage().isSliceUninitialized(theme_base, max_themes * Runtime.device().led_count / 2);

  return paletteEmpty && themeEmpty;
}

uint8_t LEDPaletteTheme::getPaletteSize() {
  return LEDPaletteTheme::palette_size_;
}

EventHandlerResult LEDPaletteTheme::onFocusEvent(const char *input) {
  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  const char *cmd = PSTR("palette");

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd);

  if (!::Focus.inputMatchesCommand(input, cmd))
    return EventHandlerResult::OK;

  if (::Focus.isEOL()) {
    for (uint8_t i = 0; i < palette_size_; i++) {
      cRGB color;

      color = lookupPaletteColor(i);
      ::Focus.send(color);
    }
    return EventHandlerResult::EVENT_CONSUMED;
  }

  uint8_t i = 0;
  while (i < palette_size_ && !::Focus.isEOL()) {
    cRGB color;

    ::Focus.read(color);
    updatePaletteColor(i, color);
    i++;
  }
  Runtime.storage().commit();

  ::LEDControl.refreshAll();

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult LEDPaletteTheme::themeFocusEvent(const char *input,
                                                    const char *expected_input,
                                                    uint16_t theme_base,
                                                    uint8_t max_themes) {
  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(expected_input);

  if (!::Focus.inputMatchesCommand(input, expected_input))
    return EventHandlerResult::OK;

  uint16_t max_index = (max_themes * Runtime.device().led_count) / 2;

  if (::Focus.isEOL()) {
    for (uint16_t pos = 0; pos < max_index; pos++) {
      uint8_t indexes = Runtime.storage().read(theme_base + pos);

      ::Focus.send((uint8_t)(indexes >> 4), indexes & ~0xf0);
    }
    return EventHandlerResult::EVENT_CONSUMED;
  }

  uint16_t pos = 0;

  while (!::Focus.isEOL() && (pos < max_index)) {
    uint8_t idx1, idx2;
    ::Focus.read(idx1);
    ::Focus.read(idx2);

    uint8_t indexes = (idx1 << 4) + idx2;

    Runtime.storage().update(theme_base + pos, indexes);
    pos++;
  }
  Runtime.storage().commit();

  ::LEDControl.refreshAll();

  return EventHandlerResult::EVENT_CONSUMED;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::LEDPaletteTheme LEDPaletteTheme;

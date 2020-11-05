/* -*- mode: c++ -*-
 * Kaleidoscope-LED-Palette-Theme -- Palette-based LED theme foundation
 * Copyright (C) 2017, 2018, 2019  Keyboard.io, Inc
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

#include <Kaleidoscope-LED-Palette-Theme.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-FocusSerial.h>

namespace kaleidoscope {
namespace plugin {

uint16_t LEDPaletteTheme::palette_base_;

uint16_t LEDPaletteTheme::reserveThemes(uint8_t max_themes) {
  if (!palette_base_)
    palette_base_ = ::EEPROMSettings.requestSlice(16 * sizeof(cRGB));

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
  uint8_t pos = Runtime.device().getLedIndex(key_addr);

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
    indexes = (other << 4) + color_index;
  } else {
    uint8_t other = indexes & ~0xf0;
    indexes = (color_index << 4) + other;
  }
  Runtime.storage().update(map_base + position / 2, indexes);
  Runtime.storage().commit();
}

EventHandlerResult LEDPaletteTheme::onFocusEvent(const char *command) {
  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  const char *cmd = PSTR("palette");

  if (::Focus.handleHelp(command, cmd))
    return EventHandlerResult::OK;

  if (strcmp_P(command, cmd) != 0)
    return EventHandlerResult::OK;

  if (::Focus.isEOL()) {
    for (uint8_t i = 0; i < 16; i++) {
      cRGB color;

      color = lookupPaletteColor(i);
      ::Focus.send(color);
    }
    return EventHandlerResult::EVENT_CONSUMED;
  }

  uint8_t i = 0;
  while (i < 16 && !::Focus.isEOL()) {
    cRGB color;

    ::Focus.read(color);
    color.r ^= 0xff;
    color.g ^= 0xff;
    color.b ^= 0xff;

    Runtime.storage().put(palette_base_ + i * sizeof(color), color);
    i++;
  }
  Runtime.storage().commit();

  ::LEDControl.refreshAll();

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult LEDPaletteTheme::themeFocusEvent(const char *command,
                                                    const char *expected_command,
                                                    uint16_t theme_base,
                                                    uint8_t max_themes) {
  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  if (::Focus.handleHelp(command, expected_command))
    return EventHandlerResult::OK;

  if (strcmp_P(command, expected_command) != 0)
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

}
}

kaleidoscope::plugin::LEDPaletteTheme LEDPaletteTheme;

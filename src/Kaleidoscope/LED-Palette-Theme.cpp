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

#include <Kaleidoscope-LED-Palette-Theme.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-Focus.h>
#include <EEPROM.h>

namespace kaleidoscope {

uint16_t LEDPaletteTheme::palette_base_;
uint8_t LEDPaletteTheme::transparent_index = 15;

LEDPaletteTheme::LEDPaletteTheme(void) {
}

void LEDPaletteTheme::begin(void) {
  USE_PLUGINS(&::EEPROMSettings);

  if (!palette_base_)
    palette_base_ = ::EEPROMSettings.requestSlice(16 * sizeof(cRGB));
}

uint16_t LEDPaletteTheme::reserveThemes(uint8_t max_themes) {
  return ::EEPROMSettings.requestSlice(max_themes * ROWS * COLS / 2);
}

void LEDPaletteTheme::updateHandler(uint16_t theme_base, uint8_t theme) {
  uint16_t map_base = theme_base + (theme * ROWS * COLS / 2);

  for (uint16_t pos = 0; pos < ROWS * COLS; pos++) {
    cRGB color;

    if (!lookupColorAtPosition(map_base, pos, &color))
      continue;

    LEDControl.led_set_crgb_at(pos, color);
  }
}

const uint8_t LEDPaletteTheme::lookupColorIndexAtPosition(uint16_t map_base, uint16_t position) {
  uint8_t color_index;

  color_index = EEPROM.read(map_base + position / 2);
  if (position % 2)
    color_index &= ~0xf0;
  else
    color_index >>= 4;

  return color_index;
}

const bool LEDPaletteTheme::lookupColorAtPosition(uint16_t map_base, uint16_t position, cRGB *color) {
  uint8_t color_index = lookupColorIndexAtPosition(map_base, position);

  if (color_index == transparent_index)
    return false;

  *color = lookupPaletteColor(color_index);

  return true;
}

const cRGB LEDPaletteTheme::lookupPaletteColor(uint8_t color_index) {
  cRGB color;

  EEPROM.get(palette_base_ + color_index * sizeof(cRGB), color);
  return color;
}

void LEDPaletteTheme::updateColorIndexAtPosition(uint16_t map_base, uint16_t position, uint8_t color_index) {
  uint8_t indexes;

  indexes = EEPROM.read(map_base + position / 2);
  if (position % 2) {
    uint8_t other = indexes >> 4;
    indexes = (other << 4) + color_index;
  } else {
    uint8_t other = indexes & ~0xf0;
    indexes = (color_index << 4) + other;
  }
  EEPROM.update(map_base + position / 2, indexes);
}

bool LEDPaletteTheme::paletteFocusHook(const char *command) {
  if (strcmp_P(command, PSTR("palette")) != 0)
    return false;

  if (Serial.peek() == '\n') {
    for (uint8_t i = 0; i < 16; i++) {
      cRGB color;

      EEPROM.get(palette_base_ + i * sizeof(color), color);
      ::Focus.printColor(color.r, color.g, color.b);
      ::Focus.printSpace();
    }
    Serial.println();
    return true;
  }

  uint8_t i = 0;
  while (i < 16 && Serial.peek() != '\n') {
    cRGB color;

    color.r = Serial.parseInt();
    color.g = Serial.parseInt();
    color.b = Serial.parseInt();

    EEPROM.put(palette_base_ + i * sizeof(color), color);
    i++;
  }

  return true;
}

bool LEDPaletteTheme::themeFocusHandler(const char *command, const char *expected_command,
                                        uint16_t theme_base, uint8_t max_themes) {
  if (strcmp_P(command, expected_command) != 0)
    return false;

  uint16_t max_index = (max_themes * ROWS * COLS) / 2;

  if (Serial.peek() == '\n') {
    for (uint16_t pos = 0; pos < max_index; pos++) {
      uint8_t indexes = EEPROM.read(theme_base + pos);

      ::Focus.printNumber(indexes >> 4);
      ::Focus.printSpace();
      ::Focus.printNumber(indexes & ~0xf0);
      ::Focus.printSpace();
    }
    Serial.println();
    return true;
  }

  uint16_t pos = 0;

  while ((Serial.peek() != '\n') && (pos < max_index)) {
    uint8_t idx1 = Serial.parseInt();
    uint8_t idx2 = Serial.parseInt();
    uint8_t indexes = (idx1 << 4) + idx2;

    EEPROM.update(theme_base + pos, indexes);
    pos++;
  }

  return true;
}

}

kaleidoscope::LEDPaletteTheme LEDPaletteTheme;

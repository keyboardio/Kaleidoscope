/* -*- mode: c++ -*-
 * Kaleidoscope-FingerPainter -- On-the-fly keyboard painting.
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

#include <Kaleidoscope-FingerPainter.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-Focus.h>

namespace KaleidoscopePlugins {
  uint16_t FingerPainter::paletteBase;
  uint16_t FingerPainter::colorBase;
  bool FingerPainter::editMode;
  const cRGB *FingerPainter::defaultPalette;

  FingerPainter::FingerPainter (void) {
  }

  void
  FingerPainter::begin (void) {
    LEDMode::begin ();
    event_handler_hook_use (eventHandlerHook);
  }

  void
  FingerPainter::configure (void) {
    paletteBase = ::EEPROMSettings.requestSlice (16 * sizeof (cRGB));
    colorBase = ::EEPROMSettings.requestSlice (ROWS * COLS);
  }

  cRGB
  FingerPainter::lookupColor (uint8_t row, uint8_t column) {
    uint16_t location = row * COLS + column;
    uint8_t colorIndex = EEPROM.read (colorBase + location);

    if (colorIndex == 0xff)
      colorIndex = 0;

    cRGB color;
    EEPROM.get (paletteBase + colorIndex * sizeof (cRGB), color);

    return color;
  }

  void
  FingerPainter::update (void) {
    for (uint8_t r = 0; r < ROWS; r++) {
      for (uint8_t c = 0; c < COLS; c++) {
        cRGB color = lookupColor (r, c);
        LEDControl.led_set_crgb_at (r, c, color);
      }
    }
  }

  void
  FingerPainter::toggleEdit (void) {
    editMode = !editMode;
  }

  Key
  FingerPainter::eventHandlerHook (Key mappedKey, byte row, byte col, uint8_t keyState) {
    if (!editMode)
      return mappedKey;

    if (row >= ROWS || col >= COLS)
      return Key_NoKey;

    if (!key_toggled_on (keyState))
      return Key_NoKey;

    cRGB oldColor, newColor;

    uint16_t location = row * COLS + col;
    uint8_t colorIndex = EEPROM.read (colorBase + location);
    if (colorIndex == 255)
      colorIndex = 0;
    EEPROM.get (paletteBase + colorIndex * sizeof (cRGB), oldColor);
    colorIndex++;
    if (colorIndex > 15)
      colorIndex = 0;
    EEPROM.get (paletteBase + colorIndex * sizeof (cRGB), newColor);
    if (memcmp (&oldColor, &newColor, sizeof (cRGB)) == 0)
      colorIndex = 0;
    EEPROM.update (colorBase + location, colorIndex);

    return Key_NoKey;
  }

  bool
  FingerPainter::focusHook (const char *command) {
    enum {
      PALETTE,
      TOGGLE,
    } subCommand;

    if (strncmp_P (command, PSTR ("fingerpainter."), 14) != 0)
      return false;

    if (strcmp_P (command + 14, PSTR ("palette")) == 0)
      subCommand = PALETTE;
    else if (strcmp_P (command + 14, PSTR ("toggle")) == 0)
      subCommand = TOGGLE;
    else
      return false;

    if (subCommand == TOGGLE) {
      ::FingerPainter.activate ();
      toggleEdit ();
      return true;
    }

    if (Serial.peek () == '\n') {
      for (uint8_t i = 0; i < 16; i++) {
        cRGB color;

        EEPROM.get (paletteBase + i * sizeof (color), color);
        ::Focus.printColor (color);

        if (i < 15)
          ::Focus.printSeparator ();
      }

      Serial.println ();
      return true;
    }

    if (Serial.peek() == 'd') {
      Serial.read ();
      if (!defaultPalette)
        return true;

      for (uint8_t i = 0; i < 16; i++) {
        EEPROM.put (paletteBase + i * sizeof (cRGB), defaultPalette[i]);
      }
      return true;
    }

    uint8_t i = 0;
    while (i < 16 && Serial.peek() != '\n') {
      cRGB color;

      color.r = Serial.parseInt ();
      color.g = Serial.parseInt ();
      color.b = Serial.parseInt ();

      EEPROM.put (paletteBase + i * sizeof (color), color);
      i++;
    }

    return true;
  }
};

KaleidoscopePlugins::FingerPainter FingerPainter;

/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Colormap -- Per-layer colormap effect
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

#include <Kaleidoscope-EEPROM-Colormap.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-Focus.h>
#include <EEPROM.h>

namespace KaleidoscopePlugins {

  uint16_t EEPROMColormapEffect::paletteBase;
  uint16_t EEPROMColormapEffect::mapBase;
  uint8_t EEPROMColormapEffect::maxLayers;

  EEPROMColormapEffect::EEPROMColormapEffect (void) {
  }

  void
  EEPROMColormapEffect::configure (uint8_t maxLayers_) {
    USE_PLUGINS (&::EEPROMSettings);

    maxLayers = maxLayers_;
    paletteBase = ::EEPROMSettings.requestSlice (15 * sizeof (cRGB));
    mapBase = ::EEPROMSettings.requestSlice (maxLayers * ROWS * COLS / 2);
  }

  void
  EEPROMColormapEffect::update (void) {
    for (uint8_t l = 0; l < 32; l++) {
      if (!Layer.isOn (l))
        continue;

      for (uint8_t r = 0; r < ROWS; r++) {
        for (uint8_t c = 0; c < COLS; c++) {
          cRGB color;

          if (!lookupColor (l, r, c, &color))
            continue;

          LEDControl.led_set_crgb_at (r, c, color);
        }
      }
    }
  }

  const bool
  EEPROMColormapEffect::lookupColor (uint8_t layer, uint8_t row, uint8_t column, cRGB *color) {
    uint8_t colorIndex;
    uint16_t mapIndex = (layer * ROWS * COLS / 2 + row * COLS / 2 + column / 2);

    colorIndex = EEPROM.read (mapBase + mapIndex);
    if (column % 2)
      colorIndex &= ~0xf0;
    else
      colorIndex >>= 4;

    if (colorIndex == Transparent)
      return false;

    EEPROM.get (paletteBase + colorIndex * sizeof (cRGB), *color);

    return true;
  }

  bool
  EEPROMColormapEffect::focusHook (const char *command) {
    enum {
      PALETTE,
      MAP,
    } subCommand;

    if (strncmp_P (command, PSTR ("colormap."), 9) != 0)
      return false;
    if (strcmp_P (command + 9, PSTR ("palette")) == 0)
      subCommand = PALETTE;
    else if (strcmp_P (command + 9, PSTR ("map")) == 0)
      subCommand = MAP;
    else
      return false;

    switch (subCommand) {
    case PALETTE:
      {
        if (Serial.peek () == '\n') {
          for (uint8_t i = 0; i < 15; i++) {
            cRGB color;

            EEPROM.get (paletteBase + i * sizeof (color), color);
            ::Focus.printColor (color);

            if (i < 14)
              ::Focus.printSeparator ();
          }
          Serial.println ();
          break;
        }

        uint8_t i = 0;
        while (i < 15 && Serial.peek() != '\n') {
          cRGB color;

          color.r = Serial.parseInt ();
          color.g = Serial.parseInt ();
          color.b = Serial.parseInt ();

          EEPROM.put (paletteBase + i * sizeof (color), color);
          i++;
        }

        break;
      }

    case MAP:
      {
        if (Serial.peek () == '\n') {
          for (uint8_t layer = 0; layer < maxLayers; layer++) {
            for (uint8_t row = 0; row < ROWS; row++) {
              for (uint8_t col = 0; col < COLS / 2; col++) {
                uint8_t indexes;
                uint16_t loc = (layer * ROWS * COLS / 2 + row * COLS / 2 + col);

                indexes = EEPROM.read (mapBase + loc);

                ::Focus.printNumber (indexes >> 4);
                ::Focus.printSeparator ();
                ::Focus.printNumber (indexes & ~0xf0);
                ::Focus.printSeparator ();
              }
              Serial.println ();
            }
            Serial.println ();
          }
          break;
        }

        uint16_t maxIndex = (maxLayers * ROWS * COLS) / 2;

        uint8_t loc = 0;
        while ((Serial.peek () != '\n') && (loc < maxIndex)) {
          uint8_t idx1 = Serial.parseInt ();
          uint8_t idx2 = Serial.parseInt ();
          uint8_t indexes = (idx1 << 4) + idx2;

          EEPROM.update (mapBase + loc , indexes);
          loc++;
        }
        break;
      }
    }

    Serial.read ();
    return true;
  }

};

KaleidoscopePlugins::EEPROMColormapEffect EEPROMColormapEffect;

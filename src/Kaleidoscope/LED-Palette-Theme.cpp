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

namespace KaleidoscopePlugins {

uint16_t LEDPaletteTheme::paletteBase;
uint8_t LEDPaletteTheme::transparentIndex = 15;

LEDPaletteTheme::LEDPaletteTheme (void) {
}

void
LEDPaletteTheme::begin (void) {
    USE_PLUGINS (&::EEPROMSettings);

    if (!paletteBase)
        paletteBase = ::EEPROMSettings.requestSlice (16 * sizeof (cRGB));
}

uint16_t
LEDPaletteTheme::reserveThemes (uint8_t maxThemes) {
    return ::EEPROMSettings.requestSlice (maxThemes * ROWS * COLS / 2);
}

void
LEDPaletteTheme::update (uint16_t themeBase, uint8_t theme) {
    uint16_t mapBase = themeBase + (theme * ROWS * COLS / 2);
    for (uint16_t loc = 0; loc < ROWS * COLS; loc++) {
        cRGB color;

        if (!lookupColor (mapBase, loc, &color))
            continue;

        LEDControl.led_set_crgb_at (loc, color);

    }
}

const uint8_t
LEDPaletteTheme::lookupColorIndex (uint16_t mapBase, uint16_t loc) {
    uint8_t colorIndex;

    colorIndex = EEPROM.read (mapBase + loc / 2);
    if (loc % 2)
        colorIndex &= ~0xf0;
    else
        colorIndex >>= 4;

    return colorIndex;
}

const bool
LEDPaletteTheme::lookupColor (uint16_t mapBase, uint16_t loc, cRGB *color) {
    uint8_t colorIndex = lookupColorIndex (mapBase, loc);

    if (colorIndex == transparentIndex)
        return false;

    EEPROM.get (paletteBase + colorIndex * sizeof (cRGB), *color);

    return true;
}

const cRGB
LEDPaletteTheme::lookupColor (uint8_t index) {
    cRGB color;

    EEPROM.get (paletteBase + index * sizeof (cRGB), color);
    return color;
}

void
LEDPaletteTheme::updateColor (uint16_t mapBase, uint16_t loc, uint8_t index) {
    uint8_t indexes;

    indexes = EEPROM.read (mapBase + loc / 2);
    if (loc % 2) {
        uint8_t other = indexes >> 4;
        indexes = (other << 4) + index;
    } else {
        uint8_t other = indexes & ~0xf0;
        indexes = (index << 4) + other;
    }
    EEPROM.update (mapBase + loc / 2, indexes);
}

bool
LEDPaletteTheme::paletteFocusHook (const char *command) {
    if (strcmp_P (command, PSTR ("palette")) != 0)
        return false;

    if (Serial.peek () == '\n') {
        for (uint8_t i = 0; i < 16; i++) {
            cRGB color;

            EEPROM.get (paletteBase + i * sizeof (color), color);
            ::Focus.printColor (color.r, color.g, color.b);
            ::Focus.printSpace ();
        }
        Serial.println ();
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

bool
LEDPaletteTheme::themeFocusHandler (const char *command, const char *expectedCommand,
                                    uint16_t themeBase, uint8_t maxThemes) {
    if (strcmp_P (command, expectedCommand) != 0)
        return false;

    uint16_t maxIndex = (maxThemes * ROWS * COLS) / 2;

    if (Serial.peek () == '\n') {
        for (uint16_t loc = 0; loc < maxIndex; loc++) {
            uint8_t indexes = EEPROM.read (themeBase + loc);

            ::Focus.printNumber (indexes >> 4);
            ::Focus.printSpace ();
            ::Focus.printNumber (indexes & ~0xf0);
            ::Focus.printSpace ();
        }
        Serial.println ();
        return true;
    }

    uint16_t loc = 0;

    while ((Serial.peek () != '\n') && (loc < maxIndex)) {
        uint8_t idx1 = Serial.parseInt ();
        uint8_t idx2 = Serial.parseInt ();
        uint8_t indexes = (idx1 << 4) + idx2;

        EEPROM.update (themeBase + loc, indexes);
        loc++;
    }

    return true;
}

};

KaleidoscopePlugins::LEDPaletteTheme LEDPaletteTheme;

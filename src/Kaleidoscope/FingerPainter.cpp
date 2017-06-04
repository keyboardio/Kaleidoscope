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

#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-Focus.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LED-Palette-Theme.h>

namespace KaleidoscopePlugins {

uint16_t FingerPainter::colorBase;
bool FingerPainter::editMode;

FingerPainter::FingerPainter(void) {
}

void
FingerPainter::begin(void) {
  USE_PLUGINS(&::LEDPaletteTheme);

  LEDMode::begin();
  event_handler_hook_use(eventHandlerHook);

  colorBase = ::LEDPaletteTheme.reserveThemes(1);
}

void
FingerPainter::update(void) {
  ::LEDPaletteTheme.update(colorBase, 0);
}

void
FingerPainter::toggleEdit(void) {
  editMode = !editMode;
}

Key
FingerPainter::eventHandlerHook(Key mappedKey, byte row, byte col, uint8_t keyState) {
  if (!editMode)
    return mappedKey;

  if (!key_toggled_on(keyState))
    return Key_NoKey;

  if (row >= ROWS || col >= COLS)
    return Key_NoKey;

  uint8_t colorIndex = ::LEDPaletteTheme.lookupColorIndex(colorBase, KeyboardHardware.get_led_index(row, col));

  // Find the next color in the palette that is different.
  // But do not loop forever!
  bool turnAround = false;
  cRGB oldColor = ::LEDPaletteTheme.lookupColor(colorIndex), newColor = oldColor;
  while (memcmp(&oldColor, &newColor, sizeof(cRGB)) == 0) {
    colorIndex++;
    if (colorIndex > 15) {
      colorIndex = 0;
      if (turnAround)
        break;
      turnAround = true;
    }
    newColor = ::LEDPaletteTheme.lookupColor(colorIndex);
  }

  ::LEDPaletteTheme.updateColor(colorBase, KeyboardHardware.get_led_index(row, col), colorIndex);

  return Key_NoKey;
}

bool
FingerPainter::focusHook(const char *command) {
  enum {
    TOGGLE,
    CLEAR,
  } subCommand;

  if (strncmp_P(command, PSTR("fingerpainter."), 14) != 0)
    return false;

  if (strcmp_P(command + 14, PSTR("toggle")) == 0)
    subCommand = TOGGLE;
  else if (strcmp_P(command + 14, PSTR("clear")) == 0)
    subCommand = CLEAR;
  else
    return false;

  if (subCommand == CLEAR) {
    for (uint16_t i = 0; i < ROWS * COLS / 2; i++) {
      EEPROM.update(colorBase + i, 0);
    }
    return true;
  }

  ::FingerPainter.activate();
  toggleEdit();

  return true;
}
};

KaleidoscopePlugins::FingerPainter FingerPainter;

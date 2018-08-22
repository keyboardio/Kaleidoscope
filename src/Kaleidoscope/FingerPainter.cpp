/* -*- mode: c++ -*-
 * Kaleidoscope-FingerPainter -- On-the-fly keyboard painting.
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

#include <Kaleidoscope-FingerPainter.h>

#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-Focus.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-LED-Palette-Theme.h>

namespace kaleidoscope {

uint16_t FingerPainter::color_base_;
bool FingerPainter::edit_mode_;

EventHandlerResult FingerPainter::onSetup() {
  color_base_ = ::LEDPaletteTheme.reserveThemes(1);
  ::LEDControl.mode_add(this);
  return EventHandlerResult::OK;
}

void FingerPainter::update(void) {
  ::LEDPaletteTheme.updateHandler(color_base_, 0);
}

void FingerPainter::refreshAt(byte row, byte col) {
  ::LEDPaletteTheme.refreshAt(color_base_, 0, row, col);
}

void FingerPainter::toggle(void) {
  edit_mode_ = !edit_mode_;
}

EventHandlerResult FingerPainter::onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t key_state) {
  if (!edit_mode_)
    return EventHandlerResult::OK;

  if (!keyToggledOn(key_state)) {
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (row >= ROWS || col >= COLS)
    return EventHandlerResult::EVENT_CONSUMED;

  uint8_t color_index = ::LEDPaletteTheme.lookupColorIndexAtPosition(color_base_, KeyboardHardware.getLedIndex(row, col));

  // Find the next color in the palette that is different.
  // But do not loop forever!
  bool turn_around = false;
  cRGB old_color = ::LEDPaletteTheme.lookupPaletteColor(color_index), new_color = old_color;
  while (memcmp(&old_color, &new_color, sizeof(cRGB)) == 0) {
    color_index++;
    if (color_index > 15) {
      color_index = 0;
      if (turn_around)
        break;
      turn_around = true;
    }
    new_color = ::LEDPaletteTheme.lookupPaletteColor(color_index);
  }

  ::LEDPaletteTheme.updateColorIndexAtPosition(color_base_, KeyboardHardware.getLedIndex(row, col), color_index);

  return EventHandlerResult::EVENT_CONSUMED;
}

bool FingerPainter::focusHook(const char *command) {
  enum {
    TOGGLE,
    CLEAR,
  } sub_command;

  if (strncmp_P(command, PSTR("fingerpainter."), 14) != 0)
    return false;

  if (strcmp_P(command + 14, PSTR("toggle")) == 0)
    sub_command = TOGGLE;
  else if (strcmp_P(command + 14, PSTR("clear")) == 0)
    sub_command = CLEAR;
  else
    return false;

  if (sub_command == CLEAR) {
    for (uint16_t i = 0; i < ROWS * COLS / 2; i++) {
      EEPROM.update(color_base_ + i, 0);
    }
    return true;
  }

  ::FingerPainter.activate();
  toggle();

  return true;
}

}

kaleidoscope::FingerPainter FingerPainter;

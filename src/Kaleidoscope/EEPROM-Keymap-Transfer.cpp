/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Keymap -- EEPROM-based keymap support.
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

#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-EEPROM-Keymap.h>
#include <Kaleidoscope-FocusSerial.h>

namespace kaleidoscope {
namespace eeprom {

EventHandlerResult FocusKeymapTransferCommand::onFocusEvent(const char *command) {
  const char *cmd = PSTR("keymap.transfer");

  if (::Focus.handleHelp(command, cmd))
    return EventHandlerResult::OK;
  if (strcmp_P(command, cmd) != 0)
    return EventHandlerResult::OK;

  uint8_t layer = Serial.parseInt();

  for (uint8_t row = 0; row < ROWS; row++) {
    for (uint8_t col = 0; col < COLS; col++) {
      Key k = Layer.getKeyFromPROGMEM(layer, row, col);
      uint16_t pos = ((layer * ROWS * COLS) + (row * COLS) + col);

      ::EEPROMKeymap.updateKey(pos, k);
    }
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

}
}

kaleidoscope::eeprom::FocusKeymapTransferCommand FocusKeymapTransferCommand;

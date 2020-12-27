/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Keymap-Programmer -- On-the-fly reprogrammable keymap.
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

#pragma once

#include "kaleidoscope/Runtime.h"
#include <Kaleidoscope-EEPROM-Keymap.h>

namespace kaleidoscope {
namespace plugin {
class EEPROMKeymapProgrammer : public kaleidoscope::Plugin {
 public:
  typedef enum {
    CODE,
    COPY,
  } mode_t;
  static mode_t mode;

  EEPROMKeymapProgrammer(void) {}

  static void activate(void) {
    nextState();
  }
  static void nextState(void);
  static void cancel(void);

  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state);
  EventHandlerResult onFocusEvent(const char *command);

 private:
  typedef enum {
    INACTIVE,
    WAIT_FOR_KEY,
    WAIT_FOR_CODE,
    WAIT_FOR_SOURCE_KEY,
  } state_t;
  static state_t state_;

  static uint16_t update_position_;  // layer, row, col
  static Key new_key_;
};
}
}

extern kaleidoscope::plugin::EEPROMKeymapProgrammer EEPROMKeymapProgrammer;

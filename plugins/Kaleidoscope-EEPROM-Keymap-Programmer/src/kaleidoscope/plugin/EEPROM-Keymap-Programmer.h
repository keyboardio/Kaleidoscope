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

#include <stdint.h>  // for uint16_t

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/plugin.h"                // for Plugin

namespace kaleidoscope {
namespace plugin {
class EEPROMKeymapProgrammer : public kaleidoscope::Plugin {
 public:
  typedef enum {
    CODE,
    COPY,
  } mode_t;
  mode_t mode;

  void activate() {
    nextState();
  }
  void nextState();
  void cancel();

  EventHandlerResult onKeyEvent(KeyEvent &event);
  EventHandlerResult onFocusEvent(const char *input);

 private:
  typedef enum {
    INACTIVE,
    WAIT_FOR_KEY,
    WAIT_FOR_CODE,
    WAIT_FOR_SOURCE_KEY,
  } state_t;
  state_t state_;

  uint16_t update_position_;  // layer, row, col
  Key new_key_;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::EEPROMKeymapProgrammer EEPROMKeymapProgrammer;

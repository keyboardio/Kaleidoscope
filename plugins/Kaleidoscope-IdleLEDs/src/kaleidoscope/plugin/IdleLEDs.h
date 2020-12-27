/* -*- mode: c++ -*-
 * Kaleidoscope-Idle-LEDs -- Turn off the LEDs when the keyboard's idle
 * Copyright (C) 2018, 2019  Keyboard.io, Inc
 * Copyright (C) 2019  Dygma, Inc
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

namespace kaleidoscope {
namespace plugin {

class IdleLEDs: public kaleidoscope::Plugin {
 public:
  IdleLEDs(void) {}

  static uint32_t idle_time_limit;

  static uint32_t idleTimeoutSeconds();
  static void setIdleTimeoutSeconds(uint32_t new_limit);

  EventHandlerResult beforeEachCycle();
  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state);

 private:
  static bool idle_;
  static uint32_t start_time_;
};

class PersistentIdleLEDs : public IdleLEDs {
 public:
  EventHandlerResult onSetup();
  EventHandlerResult onFocusEvent(const char *command);

  static void setIdleTimeoutSeconds(uint32_t new_limit);
 private:
  static uint16_t settings_base_;
};

}
}

extern kaleidoscope::plugin::IdleLEDs IdleLEDs;
extern kaleidoscope::plugin::PersistentIdleLEDs PersistentIdleLEDs;

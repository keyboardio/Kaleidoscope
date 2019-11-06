/* -*- mode: c++ -*-
 * Kaleidoscope-Idle-LEDs -- Turn off the LEDs when the keyboard's idle
 * Copyright (C) 2018  Keyboard.io, Inc
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

#include <Kaleidoscope.h>
#include "kaleidoscope/focus_eeprom.h"

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

 protected:
  static uint32_t start_time_;
};

class PersistentIdleLEDs : public IdleLEDs {
 public:

  EventHandlerResult onSetup();
  EventHandlerResult onFocusEvent(const char *command);

  static void setIdleTimeoutSeconds(uint32_t new_limit);

 private:
  static ReferenceFocusEEPROMWrapper<uint32_t, uint16_t> start_time_focus_;

  // Note: If members are added that should themselves be Focus controlled
  //       and support EEPROM storage without being associated with
  //       a variable of a parent class, use the ValueFocusEEPROMWrapper
  //       instead.
  //ValueFocusEEPROMWrapper<uint32_t, uint16_t> start_time_focus_;
};

}
}

extern kaleidoscope::plugin::IdleLEDs IdleLEDs;
extern kaleidoscope::plugin::PersistentIdleLEDs PersistentIdleLEDs;

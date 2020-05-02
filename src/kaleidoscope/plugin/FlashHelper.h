/* -*- mode: c++ -*-
 * Kaleidoscope-FlashHelper -- Firmware-assisted flashing
 * Copyright (C) 2020  Keyboard.io, Inc
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

class FlashHelper: public kaleidoscope::Plugin {
 public:
  FlashHelper() {}

  static void setTimeout(uint8_t timeout) {
    timeout_ = timeout;
  }
  static uint8_t getTimeout() {
    return timeout_;
  }
  static void activate() {
    is_active_ = true;
    start_time_ = Runtime.millisAtCycleStart();
  }
  static void deactivate() {
    is_active_ = false;
  }
  static bool isActive() {
    return is_active_;
  }

  EventHandlerResult onFocusEvent(const char *command);
  EventHandlerResult onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state);
  EventHandlerResult beforeEachCycle();

 private:
  static bool is_active_;
  static uint8_t timeout_;

  static uint32_t start_time_;
};

}
}

extern kaleidoscope::plugin::FlashHelper FlashHelper;

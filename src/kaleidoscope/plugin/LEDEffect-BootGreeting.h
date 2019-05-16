/* -*- mode: c++ -*-
 * Kaleidoscope-LEDEffect-BootGreeting -- Small greeting at boot time
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

#include "Kaleidoscope-LEDControl.h"

namespace kaleidoscope {
namespace plugin {
class BootGreetingEffect : public kaleidoscope::Plugin {
 public:
  BootGreetingEffect(void) {}
  BootGreetingEffect(KeyAddr key_addr);
  DEPRECATED(ROW_COL_FUNC) BootGreetingEffect(byte row, byte col)
    : BootGreetingEffect(KeyAddr(row, col)) {}

  static KeyAddr user_key_addr;
  static Key search_key;
  static uint8_t hue;
  static uint16_t timeout;

  EventHandlerResult afterEachCycle();

 private:
  static void findLed(void);
  static bool done_;
  static KeyAddr key_addr_;
  static uint16_t start_time;
};
}
}

extern kaleidoscope::plugin::BootGreetingEffect BootGreetingEffect;

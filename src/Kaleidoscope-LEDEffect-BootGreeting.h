/* -*- mode: c++ -*-
 * Kaleidoscope-LEDEffect-BootGreeting -- Small greeting at boot time
 * Copyright (C) 2017, 2018  Gergely Nagy
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

#pragma once

#include "Kaleidoscope-LEDControl.h"

namespace kaleidoscope {
class BootGreetingEffect : public kaleidoscope::Plugin {
 public:
  BootGreetingEffect(void) {}
  BootGreetingEffect(byte, byte);

  static byte key_row;
  static byte key_col;
  static Key search_key;
  static uint8_t hue;
  static uint16_t timeout;

  EventHandlerResult afterEachCycle();

#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
  kaleidoscope::EventHandlerResult onSetup() {
    return kaleidoscope::EventHandlerResult::OK;
  }

 protected:
  void begin();
  static void legacyLoopHook(bool is_post_clear);
#endif

 private:
  static void findLed(void);
  static bool done_;
  static byte row_;
  static byte col_;
  static uint16_t start_time;
};
}

extern kaleidoscope::BootGreetingEffect BootGreetingEffect;

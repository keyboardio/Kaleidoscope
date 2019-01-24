/* -*- mode: c++ -*-
 * Kaleidoscope-Idle-LEDs -- Turn off the LEDs when the keyboard's idle
 * Copyright (C) 2018-2019  Keyboard.io, Inc
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

#include <Kaleidoscope-IdleLEDs.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-UnderglowControl.h>

namespace kaleidoscope {
namespace plugin {

uint16_t IdleLEDs::idle_time_limit = 600; // 10 minutes
uint32_t IdleLEDs::end_time_;

EventHandlerResult IdleLEDs::beforeEachCycle() {
  if (Kaleidoscope.millisAtCycleStart() < end_time_)
    return EventHandlerResult::OK;

  if (::Kaleidoscope.has_leds && !::LEDControl.paused) {
    ::LEDControl.set_all_leds_to(CRGB(0, 0, 0));
    ::LEDControl.syncLeds();

    ::LEDControl.paused = true;
  }
  if (::UnderglowControl.has_leds() && !::UnderglowControl.isPaused()) {
    ::UnderglowControl.setColor(0, 0, 0);
    ::UnderglowControl.sync();
    ::UnderglowControl.pause();
  }

  return EventHandlerResult::OK;
}

EventHandlerResult IdleLEDs::onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t key_state) {

  if (Kaleidoscope.has_leds && ::LEDControl.paused) {
    ::LEDControl.paused = false;
    ::LEDControl.refreshAll();
  }
  if (::UnderglowControl.has_leds() && ::UnderglowControl.isPaused()) {
    ::UnderglowControl.resume();
  }

  end_time_ = Kaleidoscope.millisAtCycleStart() + (uint32_t)idle_time_limit * 1000;

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::IdleLEDs IdleLEDs;

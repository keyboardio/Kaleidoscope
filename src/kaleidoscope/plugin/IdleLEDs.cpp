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

#include <Kaleidoscope-IdleLEDs.h>
#include <Kaleidoscope-LEDControl.h>

namespace kaleidoscope {
namespace plugin {

uint32_t IdleLEDs::idle_time_limit = 600000; // 10 minutes
uint32_t IdleLEDs::start_time_     = 0;

uint32_t IdleLEDs::idleTimeoutSeconds() {
  return idle_time_limit / 1000;
}

void IdleLEDs::setIdleTimeoutSeconds(uint32_t new_limit) {
  idle_time_limit = new_limit * 1000;
}

EventHandlerResult IdleLEDs::beforeEachCycle() {
  if (!::LEDControl.paused &&
      Kaleidoscope.hasTimeExpired(start_time_, idle_time_limit)) {
    ::LEDControl.set_all_leds_to(CRGB(0, 0, 0));
    ::LEDControl.syncLeds();

    ::LEDControl.paused = true;
  }

  return EventHandlerResult::OK;
}

EventHandlerResult IdleLEDs::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state) {

  if (::LEDControl.paused) {
    ::LEDControl.paused = false;
    ::LEDControl.refreshAll();
  }

  start_time_ = Kaleidoscope.millisAtCycleStart();

  return EventHandlerResult::OK;
}

ReferenceFocusEEPROMWrapper<uint32_t, uint16_t>
PersistentIdleLEDs::start_time_focus_{
  // The parent class member variable that is
  // controlled by the wrapper
  //
  PersistentIdleLEDs::start_time_,

  // Make sure to store the focus string only once
  // in PROGMEM.
  //
  PSTR_ANYWHERE("idleLeds.idleTimeLimit")};

EventHandlerResult PersistentIdleLEDs::onSetup() {
  start_time_focus_.setup();
  return EventHandlerResult::OK;
}
EventHandlerResult PersistentIdleLEDs::onFocusEvent(const char *command) {
  if (start_time_focus_.focusRead(command)) {
    return EventHandlerResult::EVENT_CONSUMED;
  }

  return EventHandlerResult::OK;
}

void PersistentIdleLEDs::setIdleTimeoutSeconds(uint32_t new_limit) {
  IdleLEDs::setIdleTimeoutSeconds(new_limit);
  start_time_focus_.store();
}

}
}

kaleidoscope::plugin::IdleLEDs IdleLEDs;
kaleidoscope::plugin::PersistentIdleLEDs PersistentIdleLEDs;

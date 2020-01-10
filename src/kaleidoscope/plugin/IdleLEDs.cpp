/* -*- mode: c++ -*-
 * Kaleidoscope-Idle-LEDs -- Turn off the LEDs when the keyboard's idle
 * Copyright (C) 2018, 2019, 2020  Keyboard.io, Inc
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

#include <Kaleidoscope-IdleLEDs.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-FocusSerial.h>

namespace kaleidoscope {
namespace plugin {

uint32_t IdleLEDs::idle_time_limit = 600000; // 10 minutes
uint32_t IdleLEDs::start_time_     = 0;
bool IdleLEDs::idle_;

uint32_t IdleLEDs::idleTimeoutSeconds() {
  return idle_time_limit / 1000;
}

void IdleLEDs::setIdleTimeoutSeconds(uint32_t new_limit) {
  idle_time_limit = new_limit * 1000;
}

EventHandlerResult IdleLEDs::beforeEachCycle() {
  if (idle_time_limit == 0)
    return EventHandlerResult::OK;

  if (::LEDControl.isEnabled() &&
      Runtime.hasTimeExpired(start_time_, idle_time_limit)) {
    ::LEDControl.disable();
    idle_ = true;
  }

  return EventHandlerResult::OK;
}

EventHandlerResult IdleLEDs::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t key_state) {

  if (idle_) {
    ::LEDControl.enable();
    idle_ = false;
  }

  start_time_ = Runtime.millisAtCycleStart();

  return EventHandlerResult::OK;
}

uint16_t PersistentIdleLEDs::settings_base_;

EventHandlerResult PersistentIdleLEDs::onSetup() {
  settings_base_ = ::EEPROMSettings.requestSlice(sizeof(uint16_t));

  // If idleTime is max, assume that EEPROM is uninitialized, and store the
  // defaults.
  uint16_t idle_time;
  Runtime.storage().get(settings_base_, idle_time);
  if (idle_time == 0xffff) {
    idle_time = idle_time_limit;
  }
  setIdleTimeoutSeconds(idle_time);

  return EventHandlerResult::OK;
}

void PersistentIdleLEDs::setIdleTimeoutSeconds(uint32_t new_limit) {
  IdleLEDs::setIdleTimeoutSeconds(new_limit);

  uint16_t stored_limit = (uint16_t)new_limit;
  Runtime.storage().put(settings_base_, stored_limit);
  Runtime.storage().commit();
}

EventHandlerResult PersistentIdleLEDs::onFocusEvent(const char *command) {
  const char *cmd = PSTR("idleleds.time_limit");

  if (::Focus.handleHelp(command, cmd))
    return EventHandlerResult::OK;

  if (strcmp_P(command, cmd) != 0)
    return EventHandlerResult::OK;

  if (::Focus.isEOL()) {
    ::Focus.send(idleTimeoutSeconds());
  } else {
    uint16_t idle_time;
    ::Focus.read(idle_time);
    setIdleTimeoutSeconds(idle_time);
  }
  return EventHandlerResult::EVENT_CONSUMED;
}

}
}

kaleidoscope::plugin::IdleLEDs IdleLEDs;
kaleidoscope::plugin::PersistentIdleLEDs PersistentIdleLEDs;

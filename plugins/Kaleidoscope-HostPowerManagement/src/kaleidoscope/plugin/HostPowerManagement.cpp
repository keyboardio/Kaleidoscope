/* -*- mode: c++ -*-
 * Kaleidoscope-HostPowerManagement -- Host power management support plugin.
 * Copyright (C) 2017, 2018  Keyboard.io, Inc.
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

#include "kaleidoscope/plugin/HostPowerManagement.h"

#include <Arduino.h>  // IWYU pragma: keep
#include <stdint.h>   // for uint8_t

#ifdef ARDUINO_ARCH_GD32
#include "USBCore.h"
#endif

#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK

namespace kaleidoscope {
namespace plugin {

bool HostPowerManagement::was_suspended_ = false;

bool HostPowerManagement::isSuspended() {
#if defined(__AVR__)
  return USBDevice.isSuspended();
#elif defined(ARDUINO_ARCH_GD32)
  return USBCore().isSuspended();
#else
  return false;
#endif
}

EventHandlerResult HostPowerManagement::beforeEachCycle() {
  if (isSuspended()) {
    if (!was_suspended_) {
      was_suspended_ = true;
      hostPowerManagementEventHandler(Suspend);
    } else {
      hostPowerManagementEventHandler(Sleep);
    }
  } else {
    if (was_suspended_) {
      was_suspended_ = false;
      hostPowerManagementEventHandler(Resume);
    }
  }

  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

__attribute__((weak)) void hostPowerManagementEventHandler(kaleidoscope::plugin::HostPowerManagement::Event event) {
}

kaleidoscope::plugin::HostPowerManagement HostPowerManagement;

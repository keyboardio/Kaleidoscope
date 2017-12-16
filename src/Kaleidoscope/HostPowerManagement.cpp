/* -*- mode: c++ -*-
 * Kaleidoscope-HostPowerManagement -- Host power management support plugin.
 * Copyright (C) 2017  Gergely Nagy
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

#include <Kaleidoscope.h>
#include <Kaleidoscope-HostPowerManagement.h>
#include <Kaleidoscope-LEDControl.h>

// This is a terrible hack until Arduino#6964 gets implemented.
// It makes the `_usbSuspendState` symbol available to us.
extern u8 _usbSuspendState;

namespace kaleidoscope {

bool HostPowerManagement::was_suspended_ = false;
bool HostPowerManagement::initial_suspend_ = true;

void HostPowerManagement::begin(void) {
  Kaleidoscope.useLoopHook(loopHook);
}

void HostPowerManagement::toggleLEDs(HostPowerManagement::Event event) {
  switch (event) {
  case Suspend:
    ::LEDControl.paused = true;
    ::LEDControl.set_all_leds_to({0, 0, 0});
    ::LEDControl.syncLeds();
    break;
  case Resume:
    ::LEDControl.paused = false;
    ::LEDControl.refreshAll();
    break;
  case Sleep:
    break;
  }
}

void HostPowerManagement::loopHook(bool post_clear) {
  if (post_clear)
    return;

  if ((_usbSuspendState & (1 << SUSPI))) {
    if (!initial_suspend_) {
      if (!was_suspended_) {
        was_suspended_ = true;
        hostPowerManagementEventHandler(Suspend);
      } else {
        hostPowerManagementEventHandler(Sleep);
      }
    }
  } else {
    if (initial_suspend_)
      initial_suspend_ = false;
    if (was_suspended_) {
      was_suspended_ = false;
      hostPowerManagementEventHandler(Resume);
    }
  }
}

}

__attribute__((weak)) void hostPowerManagementEventHandler(kaleidoscope::HostPowerManagement::Event event) {
  HostPowerManagement.toggleLEDs(event);
}

kaleidoscope::HostPowerManagement HostPowerManagement;

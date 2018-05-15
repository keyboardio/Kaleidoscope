/* -*- mode: c++ -*-
 * Kaleidoscope-HostPowerManagement -- Host power management support plugin.
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

#include <Kaleidoscope.h>
#include <Kaleidoscope-HostPowerManagement.h>
#include <Kaleidoscope-LEDControl.h>

// This is a terrible hack until Arduino#6964 gets implemented.
// It makes the `_usbSuspendState` symbol available to us.
extern u8 _usbSuspendState;

namespace kaleidoscope {

bool HostPowerManagement::was_suspended_ = false;
bool HostPowerManagement::initial_suspend_ = true;

EventHandlerResult HostPowerManagement::beforeEachCycle() {
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

  return EventHandlerResult::OK;
}

// Legacy V1 API
#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
void HostPowerManagement::begin() {
  Kaleidoscope.useLoopHook(legacyLoopHook);
}

void HostPowerManagement::legacyLoopHook(bool is_post_clear) {
  if (is_post_clear)
    return;

  ::HostPowerManagement.beforeEachCycle();
}
#endif

}

__attribute__((weak)) void hostPowerManagementEventHandler(kaleidoscope::HostPowerManagement::Event event) {
}

kaleidoscope::HostPowerManagement HostPowerManagement;

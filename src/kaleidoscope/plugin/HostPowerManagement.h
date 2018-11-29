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

#pragma once

#include <Kaleidoscope.h>

#define _DEPRECATED_MESSAGE_ENABLEWAKEUP                                    \
  "The HostPowerManagement.enableWakeup() call is not necessary anymore,\n" \
  "the firmware supports wakeup by default now. The line can be safely\n"   \
  "removed."

namespace kaleidoscope {
namespace plugin {
class HostPowerManagement : public kaleidoscope::Plugin {
 public:
  typedef enum {
    Suspend,
    Sleep,
    Resume,
  } Event;

  HostPowerManagement(void) {}

  void enableWakeup(void) DEPRECATED(ENABLEWAKEUP) {}

  EventHandlerResult beforeEachCycle();

 private:
  static bool was_suspended_;
  static bool initial_suspend_;
};
}

// Backwards compatibility
typedef plugin::HostPowerManagement HostPowerManagement;

}

void hostPowerManagementEventHandler(kaleidoscope::plugin::HostPowerManagement::Event event);

extern kaleidoscope::plugin::HostPowerManagement HostPowerManagement;

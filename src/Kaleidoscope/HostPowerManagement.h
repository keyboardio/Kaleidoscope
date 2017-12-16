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

#pragma once

#include <Kaleidoscope.h>
#include "WakeupKeyboard.h"

namespace kaleidoscope {
class HostPowerManagement : public KaleidoscopePlugin {
 public:
  typedef enum {
    Suspend,
    Sleep,
    Resume,
  } Event;

  HostPowerManagement(void) {};

  void begin(void) final;
  void enableWakeup(void) {
    WakeupKeyboard.begin();
  };

 private:
  static bool was_suspended_;
  static bool initial_suspend_;

  static void loopHook(bool post_clear);
};
}

void hostPowerManagementEventHandler(kaleidoscope::HostPowerManagement::Event event);

extern kaleidoscope::HostPowerManagement HostPowerManagement;

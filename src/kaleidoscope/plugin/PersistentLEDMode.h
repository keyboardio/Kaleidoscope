/* -*- mode: c++ -*-
 * kaleidoscope::plugin::PersistentLEDMode -- Persist the current LED mode to Storage
 * Copyright (C) 2019  Keyboard.io, Inc.
 * Copyright (C) 2019  Dygma, Inc.
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

/* NOTE: This plugin is a workaround. It allows us to (optionally) save the LED
 * mode to storage, and restore it on next boot, without having a way to hook
 * into led mode change events. Once we can hook into that, this plugin shall be
 * reworked to use it instead of keeping `afterEachCycle()` busy. */

#pragma once

#include <Kaleidoscope.h>

namespace kaleidoscope {
namespace plugin {

class PersistentLEDMode: public kaleidoscope::Plugin {
 public:
  PersistentLEDMode() {}

  EventHandlerResult onSetup();
  EventHandlerResult afterEachCycle();
 private:
  static uint16_t settings_base_;
  static uint8_t cached_mode_index_;
};

}
}

extern kaleidoscope::plugin::PersistentLEDMode PersistentLEDMode;

/* Kaleidoscope-LEDControl - LED control plugin for Kaleidoscope
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
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

#include "kaleidoscope/KeyAddr.h"         // for KeyAddr
#include "kaleidoscope/plugin/LEDMode.h"  // for LEDMode

namespace kaleidoscope {
namespace plugin {

// This is still an old style persistent LEDMode as it does not have
// any members and thus there would not be any gain from making it dynamic.
//
class LEDOff : public LEDMode {
 public:
  LEDOff() { led_mode_name_ = "Off"; }
  explicit LEDOff(const char *led_mode_name) { led_mode_name_ = led_mode_name; }

 protected:
  void onActivate() final;
  void refreshAt(KeyAddr key_addr) final;
};
}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::LEDOff LEDOff;

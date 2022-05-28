/* Kaleidoscope-HardwareTestMode - A factory test mode for the Model 01.
 * Copyright (C) 2017-2019  Keyboard.io, Inc.
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

#include <stdint.h>  // for uint8_t

#include "kaleidoscope/device/device.h"  // for cRGB
#include "kaleidoscope/plugin.h"         // for Plugin

namespace kaleidoscope {
namespace plugin {

class HardwareTestMode : public kaleidoscope::Plugin {
 public:
  typedef struct {
    uint8_t bad : 1,
      tested : 1,
      cyclesSinceStateChange : 6;
  } chatter_data;
  static uint8_t actionKey;

  static void runTests();
  static void setActionKey(uint8_t key);

 private:
  static void testLeds();
  static void testMatrix();
  static void toggleProgrammingLedsOn();
  static void waitForKeypress();
  static void setLeds(cRGB color);
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::HardwareTestMode HardwareTestMode;

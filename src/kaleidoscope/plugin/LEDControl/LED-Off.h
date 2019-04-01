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

#include "Kaleidoscope-LEDControl.h"

namespace kaleidoscope {
namespace plugin {
class LEDOff : public LEDMode {
 public:
  LEDOff(void) { }

 protected:
  void onActivate(void) final;
  void refreshAt(KeyLEDAddr led_addr) final;
  KS_ROW_COL_FUNC void refreshAt(byte row, byte col) final {
    refreshAt(KeyLEDAddr(row, col));
  }
};
}
}

extern kaleidoscope::plugin::LEDOff LEDOff;

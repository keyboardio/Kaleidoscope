/* -*- mode: c++ -*-
 * Kaleidoscope-LEDEffect-BootAnimation -- Small greeting at boot time
 * Copyright (C) 2018  Keyboard.io, Inc
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

#include "kaleidoscope/Runtime.h"
#include "Kaleidoscope-LEDControl.h"

namespace kaleidoscope {
namespace plugin {
class BootAnimationEffect : public kaleidoscope::Plugin {
 public:
  BootAnimationEffect(void) {}

  static uint16_t timeout;
  static cRGB color;

  EventHandlerResult afterEachCycle();
  EventHandlerResult onSetup();

 private:
  static const uint8_t greeting_[11];

  static bool done_;
  static uint16_t start_time_;
  static uint8_t current_index_;
};
}
}

extern kaleidoscope::plugin::BootAnimationEffect BootAnimationEffect;

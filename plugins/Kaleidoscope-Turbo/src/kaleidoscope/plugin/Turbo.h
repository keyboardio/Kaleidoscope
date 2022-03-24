/* -*- mode: c++ -*-
 * Kaleidoscope-Turbo
 * Copyright (C) 2018 ash lea
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

#include <Kaleidoscope-Ranges.h>                // for TURBO
#include <stdint.h>                             // for uint16_t, uint32_t

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/device/device.h"         // for cRGB
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/plugin.h"                // for Plugin

constexpr Key Key_Turbo = Key(kaleidoscope::ranges::TURBO);

namespace kaleidoscope {
namespace plugin {
class Turbo : public kaleidoscope::Plugin {
 public:
  Turbo() {}

  uint16_t interval();
  void interval(uint16_t newVal);

  uint16_t flashInterval();
  void flashInterval(uint16_t newVal);

  bool sticky();
  void sticky(bool newVal);

  bool flash();
  void flash(bool newVal);

  cRGB activeColor();
  void activeColor(cRGB newVal);

  EventHandlerResult onNameQuery();
  EventHandlerResult onKeyEvent(KeyEvent &event);
  EventHandlerResult afterEachCycle();
  EventHandlerResult beforeSyncingLeds();

 private:
  static uint16_t interval_;
  static uint16_t flash_interval_;
  static bool sticky_;
  static bool flash_;
  static cRGB active_color_;

  static bool active_;
  static uint32_t start_time_;
  static uint32_t flash_start_time_;
};
}
}

extern kaleidoscope::plugin::Turbo Turbo;

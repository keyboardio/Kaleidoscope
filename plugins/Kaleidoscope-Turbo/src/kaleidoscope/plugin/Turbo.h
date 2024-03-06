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

#include <Kaleidoscope-Ranges.h>  // for TURBO
#include <stdint.h>               // for uint16_t, uint32_t

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/device/device.h"         // for cRGB, CRGB
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/plugin.h"                // for Plugin
#include "kaleidoscope/plugin/FocusPlugin.h"    // for FocusPlugin
constexpr Key Key_Turbo = Key(kaleidoscope::ranges::TURBO);

namespace kaleidoscope {
namespace plugin {
class Turbo : public kaleidoscope::Plugin, public FocusPlugin {
 public:
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

  EventHandlerResult onKeyEvent(KeyEvent &event);
  EventHandlerResult afterEachCycle();
  EventHandlerResult beforeSyncingLeds();

 protected:
  const __FlashStringHelper *getPluginName() const override { F("Turbo"); }

 private:
  uint16_t interval_       = 10;
  uint16_t flash_interval_ = 69;
  bool sticky_             = false;
  bool flash_              = true;
  cRGB active_color_       = CRGB(160, 0, 0);

  bool active_               = false;
  uint32_t start_time_       = 0;
  uint32_t flash_start_time_ = 0;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::Turbo Turbo;

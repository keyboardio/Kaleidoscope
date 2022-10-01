/* -*- mode: c++ -*-
 * Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2022  Keyboard.io, Inc.
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

#include <stdint.h>  // for uint8_t, uint16_t

#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin

namespace kaleidoscope {
namespace plugin {

class LEDBrightnessConfig : public kaleidoscope::Plugin {
 public:
  EventHandlerResult onSetup();
  EventHandlerResult onNameQuery();
  EventHandlerResult onFocusEvent(const char *command);

 private:
  static uint16_t settings_base_;
  static struct settings {
    uint8_t brightness;
  } settings_;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::LEDBrightnessConfig LEDBrightnessConfig;

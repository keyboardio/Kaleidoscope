/* -*- mode: c++ -*-
 * Kaleidoscope-HostOS -- Host OS detection and tracking for Kaleidoscope
 * Copyright (C) 2016-2021  Keyboard.io, Inc
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

#include <stdint.h>  // for uint16_t

#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin

namespace kaleidoscope {
namespace plugin {
namespace hostos {

typedef enum {
  LINUX,
  MACOS,
  OSX = MACOS,
  WINDOWS,
  OTHER,

  UNKNOWN = 0xff,
  AUTO    = UNKNOWN
} Type;

}

class HostOS : public kaleidoscope::Plugin {
 public:
  EventHandlerResult onSetup();

  hostos::Type os() {
    return os_;
  }
  void os(hostos::Type new_os);

 private:
  hostos::Type os_;
  uint16_t eeprom_slice_;
  bool is_configured_ = false;
};

}  // namespace plugin

namespace hostos = plugin::hostos;

}  // namespace kaleidoscope

extern kaleidoscope::plugin::HostOS HostOS;

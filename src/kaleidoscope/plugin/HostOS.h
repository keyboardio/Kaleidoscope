/* -*- mode: c++ -*-
 * Kaleidoscope-HostOS -- Host OS detection and tracking for Kaleidoscope
 * Copyright (C) 2016, 2017, 2018  Keyboard.io, Inc
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

namespace kaleidoscope {
namespace plugin {
namespace hostos {

typedef enum {
  LINUX,
  OSX,
  WINDOWS,
  OTHER,

  UNKNOWN = 0xff,
  AUTO = UNKNOWN
} Type;

}

class HostOS : public kaleidoscope::Plugin {
 public:
  HostOS() {}
  EventHandlerResult onSetup();

  hostos::Type os(void) {
    return os_;
  }
  void os(hostos::Type new_os);

 private:
  hostos::Type os_;
  uint16_t eeprom_slice_;
  bool is_configured_ = false;
};
}

namespace hostos = plugin::hostos;
}

extern kaleidoscope::plugin::HostOS HostOS;

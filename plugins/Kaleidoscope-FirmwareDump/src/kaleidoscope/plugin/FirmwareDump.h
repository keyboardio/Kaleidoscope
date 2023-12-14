/* -*- mode: c++ -*-
 * Kaleidoscope-FirmwareDump -- Firmware dumper for Kaleidoscope
 * Copyright (C) 2019  Keyboard.io, Inc.
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

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD
#warning "Firmware Dump is not available for virtual builds"
#else

#ifdef __AVR__

#include <stdint.h>  // for uint16_t

#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin

namespace kaleidoscope {
namespace plugin {

class FirmwareDump : public kaleidoscope::Plugin {
 public:
  EventHandlerResult onSetup();
  EventHandlerResult onFocusEvent(const char *input);

 private:
  uint16_t bootloader_size_;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::FirmwareDump FirmwareDump;

#endif
#endif

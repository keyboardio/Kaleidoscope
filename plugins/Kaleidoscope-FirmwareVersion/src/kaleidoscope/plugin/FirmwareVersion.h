/* -*- mode: c++ -*-
 * Kaleidoscope-FirmwareVersion -- Provides a Focus command to print a version
 * Copyright (C) 2022  Keyboard.io, Inc
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

#ifndef KALEIDOSCOPE_FIRMWARE_VERSION
#define KALEIDOSCOPE_FIRMWARE_VERSION "0.0.0"
#endif

#include <Arduino.h>                            // for PSTR, F, __FlashStringHelper
#include "Kaleidoscope-FocusSerial.h"           // for Focus, FocusSerial
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin

namespace kaleidoscope {
namespace plugin {

class FirmwareVersion : public Plugin {
 public:
  EventHandlerResult onFocusEvent(const char *input) {
    const char *cmd_version = PSTR("version");

    if (::Focus.inputMatchesHelp(input))
      return ::Focus.printHelp(cmd_version);

    if (!::Focus.inputMatchesCommand(input, cmd_version))
      return EventHandlerResult::OK;

#ifdef KALEIDOSCOPE_FIRMWARE_VERSION
    ::Focus.send(F(KALEIDOSCOPE_FIRMWARE_VERSION));
#else
    ::Focus.send(F("0.0.0"));
#endif

    return EventHandlerResult::OK;
  }
};

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::FirmwareVersion FirmwareVersion;

/* -*- mode: c++ -*-
 * kaleidoscope::device::dygma::Raise -- Kaleidoscope device plugin for Dygma Raise
 * Copyright (C) 2017-2019  Keyboard.io, Inc
 * Copyright (C) 2017-2019  Dygma Lab S.L.
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

#ifdef ARDUINO_SAMD_RAISE

#include "kaleidoscope/device/dygma/raise/Focus.h"

#include <Kaleidoscope-FocusSerial.h>

#include "kaleidoscope/Runtime.h"

namespace kaleidoscope {
namespace device {
namespace dygma {
namespace raise {

#ifndef RAISE_FIRMWARE_VERSION
#define RAISE_FIRMWARE_VERSION "<unknown>"
#endif

EventHandlerResult Focus::onFocusEvent(const char *input) {
  const char *cmd_version      = PSTR("hardware.version");
  const char *cmd_side_power   = PSTR("hardware.side_power");
  const char *cmd_side_ver     = PSTR("hardware.side_ver");
  const char *cmd_sled_ver     = PSTR("hardware.sled_ver");
  const char *cmd_sled_current = PSTR("hardware.sled_current");
  const char *cmd_layout       = PSTR("hardware.layout");
  const char *cmd_joint        = PSTR("hardware.joint");
  const char *cmd_keyscan      = PSTR("hardware.keyscan");
  const char *cmd_crc_errors   = PSTR("hardware.crc_errors");
  const char *cmd_firmware     = PSTR("hardware.firmware");

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd_version,
                             cmd_side_power,
                             cmd_side_ver,
                             cmd_sled_ver,
                             cmd_sled_current,
                             cmd_layout,
                             cmd_joint,
                             cmd_keyscan,
                             cmd_crc_errors,
                             cmd_firmware);

  if (::Focus.inputMatchesCommand(input, cmd_version)) {
    ::Focus.send("Dygma Raise");
    return EventHandlerResult::EVENT_CONSUMED;
  } else if (::Focus.inputMatchesCommand(input, cmd_firmware)) {
    ::Focus.send(RAISE_FIRMWARE_VERSION);
    return EventHandlerResult::EVENT_CONSUMED;
  } else if (::Focus.inputMatchesCommand(input, cmd_side_power)) {
    if (::Focus.isEOL()) {
      ::Focus.send(Runtime.device().side.getPower());
      return EventHandlerResult::EVENT_CONSUMED;
    } else {
      uint8_t power;
      ::Focus.read(power);
      Runtime.device().side.setPower(power);
      return EventHandlerResult::EVENT_CONSUMED;
    }
  } else if (::Focus.inputMatchesCommand(input, cmd_side_ver)) {
    ::Focus.send("left:");
    ::Focus.send(Runtime.device().side.leftVersion());
    ::Focus.send("\r\nright:");
    ::Focus.send(Runtime.device().side.rightVersion());
    return EventHandlerResult::EVENT_CONSUMED;
  } else if (::Focus.inputMatchesCommand(input, cmd_crc_errors)) {
    ::Focus.send("left:");
    ::Focus.send(Runtime.device().side.leftCRCErrors());
    ::Focus.send("\r\nright:");
    ::Focus.send(Runtime.device().side.rightCRCErrors());
    return EventHandlerResult::EVENT_CONSUMED;
  } else if (::Focus.inputMatchesCommand(input, cmd_sled_ver)) {
    ::Focus.send("left:");
    ::Focus.send(Runtime.device().side.leftSLEDVersion());
    ::Focus.send("\r\nright:");
    ::Focus.send(Runtime.device().side.rightSLEDVersion());
    return EventHandlerResult::EVENT_CONSUMED;
  } else if (::Focus.inputMatchesCommand(input, cmd_sled_current)) {
    if (::Focus.isEOL()) {
      ::Focus.send("left:");
      ::Focus.send(Runtime.device().side.leftSLEDCurrent());
      ::Focus.send("\r\nright:");
      ::Focus.send(Runtime.device().side.rightSLEDCurrent());
      return EventHandlerResult::EVENT_CONSUMED;
    } else {
      uint8_t current;
      ::Focus.read(current);
      Runtime.device().side.setSLEDCurrent(current);
      return EventHandlerResult::EVENT_CONSUMED;
    }
  } else if (::Focus.inputMatchesCommand(input, cmd_layout)) {
    static const auto ANSI = Runtime.device().settings.Layout::ANSI;
    ::Focus.send(Runtime.device().settings.layout() == ANSI ? "ANSI" : "ISO");
    return EventHandlerResult::EVENT_CONSUMED;
  } else if (::Focus.inputMatchesCommand(input, cmd_joint)) {
    ::Focus.send(Runtime.device().settings.joint());
    return EventHandlerResult::EVENT_CONSUMED;
  } else if (::Focus.inputMatchesCommand(input, cmd_keyscan)) {
    if (::Focus.isEOL()) {
      ::Focus.send(Runtime.device().settings.keyscanInterval());
      return EventHandlerResult::EVENT_CONSUMED;
    } else {
      uint8_t keyscan;
      ::Focus.read(keyscan);
      Runtime.device().settings.keyscanInterval(keyscan);
      return EventHandlerResult::EVENT_CONSUMED;
    }
  }

  return EventHandlerResult::OK;
}

}  // namespace raise
}  // namespace dygma
}  // namespace device
}  // namespace kaleidoscope

kaleidoscope::device::dygma::raise::Focus RaiseFocus;

#endif

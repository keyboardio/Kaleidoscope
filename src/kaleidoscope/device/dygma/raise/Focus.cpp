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

#include <Kaleidoscope.h>
#include <Kaleidoscope-FocusSerial.h>
#include "kaleidoscope/device/dygma/raise/Focus.h"

namespace kaleidoscope {
namespace device {
namespace dygma {
namespace raise {

#ifndef RAISE_FIRMWARE_VERSION
#define RAISE_FIRMWARE_VERSION "<unknown>"
#endif

EventHandlerResult Focus::onFocusEvent(const char *command) {
  if (::Focus.handleHelp(command, PSTR("hardware.version\nhardware.side_power\nhardware.side_ver\nhardware.sled_ver\nhardware.sled_current\nhardware.layout\nhardware.joint\nhardware.keyscan\nhardware.crc_errors\nhardware.firmware")))
    return EventHandlerResult::OK;

  if (strncmp_P(command, PSTR("hardware."), 9) != 0)
    return EventHandlerResult::OK;

  if (strcmp_P(command + 9, PSTR("version")) == 0) {
    ::Focus.send("Dygma Raise");
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (strcmp_P(command + 9, PSTR("firmware")) == 0) {
    ::Focus.send(RAISE_FIRMWARE_VERSION);
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (strcmp_P(command + 9, PSTR("side_power")) == 0)
    if (::Focus.isEOL()) {
      ::Focus.send(Kaleidoscope.device().side.getPower());
      return EventHandlerResult::EVENT_CONSUMED;
    } else {
      uint8_t power;
      ::Focus.read(power);
      Kaleidoscope.device().side.setPower(power);
      return EventHandlerResult::EVENT_CONSUMED;
    }

  if (strcmp_P(command + 9, PSTR("side_ver")) == 0) {
    ::Focus.send("left:");
    ::Focus.send(Kaleidoscope.device().side.leftVersion());
    ::Focus.send("\nright:");
    ::Focus.send(Kaleidoscope.device().side.rightVersion());
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (strcmp_P(command + 9, PSTR("crc_errors")) == 0) {
    ::Focus.send("left:");
    ::Focus.send(Kaleidoscope.device().side.leftCRCErrors());
    ::Focus.send("\nright:");
    ::Focus.send(Kaleidoscope.device().side.rightCRCErrors());
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (strcmp_P(command + 9, PSTR("sled_ver")) == 0) {
    ::Focus.send("left:");
    ::Focus.send(Kaleidoscope.device().side.leftSLEDVersion());
    ::Focus.send("\nright:");
    ::Focus.send(Kaleidoscope.device().side.rightSLEDVersion());
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (strcmp_P(command + 9, PSTR("sled_current")) == 0)
    if (::Focus.isEOL()) {
      ::Focus.send("left:");
      ::Focus.send(Kaleidoscope.device().side.leftSLEDCurrent());
      ::Focus.send("\nright:");
      ::Focus.send(Kaleidoscope.device().side.rightSLEDCurrent());
      return EventHandlerResult::EVENT_CONSUMED;
    } else {
      uint8_t current;
      ::Focus.read(current);
      Kaleidoscope.device().side.setSLEDCurrent(current);
      return EventHandlerResult::EVENT_CONSUMED;
    }

  if (strcmp_P(command + 9, PSTR("layout")) == 0) {
    static const auto ANSI = Kaleidoscope.device().settings.Layout::ANSI;
    ::Focus.send(Kaleidoscope.device().settings.layout() == ANSI ? "ANSI" : "ISO");
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (strcmp_P(command + 9, PSTR("joint")) == 0) {
    ::Focus.send(Kaleidoscope.device().settings.joint());
    return EventHandlerResult::EVENT_CONSUMED;
  }

  if (strcmp_P(command + 9, PSTR("keyscan")) == 0) {
    if (::Focus.isEOL()) {
      ::Focus.send(Kaleidoscope.device().settings.keyscanInterval());
      return EventHandlerResult::EVENT_CONSUMED;
    } else {
      uint8_t keyscan;
      ::Focus.read(keyscan);
      Kaleidoscope.device().settings.keyscanInterval(keyscan);
      return EventHandlerResult::EVENT_CONSUMED;
    }
  }

  return EventHandlerResult::OK;
}

}
}
}
}

kaleidoscope::device::dygma::raise::Focus RaiseFocus;

#endif

/* -*- mode: c++ -*-
 * Kaleidoscope-MouseKeys -- Mouse keys for Kaleidoscope.
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

#include "kaleidoscope/plugin/MouseKeys.h"  // IWYU pragma: associated

#include <Arduino.h>                       // for PSTR, strcmp_P, strncmp_P
#include <Kaleidoscope-EEPROM-Settings.h>  // for EEPROMSettings
#include <Kaleidoscope-FocusSerial.h>      // for Focus, FocusSerial
#include <stdint.h>                        // for uint16_t, uint32_t, uint8_t

#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for VirtualProps::Storage, Base<>::Storage
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK

namespace kaleidoscope {
namespace plugin {

// =============================================================================
// MouseKeys configurator

EventHandlerResult MouseKeysConfig::onSetup() {
  settings_addr_ = ::EEPROMSettings.requestSlice(sizeof(MouseKeys::Settings));
  uint32_t checker;

  Runtime.storage().get(settings_addr_, checker);

  // If the EEPROM is empty, storre the default settings.
  if (checker == 0xffffffff) {
    Runtime.storage().put(settings_addr_, ::MouseKeys.settings_);
    Runtime.storage().commit();
  }

  Runtime.storage().get(settings_addr_, ::MouseKeys.settings_);
  return EventHandlerResult::OK;
}

// -----------------------------------------------------------------------------
EventHandlerResult MouseKeysConfig::onFocusEvent(const char *command) {
  // If the focus command is a request for help, provide the list of valid
  // commands.
  if (::Focus.handleHelp(command, PSTR("mousekeys.scroll_interval\n"
                                       "mousekeys.init_speed\n"
                                       "mousekeys.base_speed\n"
                                       "mousekeys.accel_duration")))
    return EventHandlerResult::OK;

  // The length of the string `mousekeys.`:
  constexpr uint8_t base_cmd_len = 10;

  // If this is not a MouseKeys command, do nothing.
  if (strncmp_P(command, PSTR("mousekeys."), base_cmd_len) != 0)
    return EventHandlerResult::OK;
  // Otherwise, advance the pointer to the subcommand.
  command += base_cmd_len;

  enum Command : uint8_t {
    SCROLL_INTERVAL,
    INIT_SPEED,
    BASE_SPEED,
    ACCEL_DURATION,
  };
  Command cmd;

  // Parse the (sub)command.  If it's not a valid command, abort.
  if (strcmp_P(command, PSTR("scroll_interval")) == 0)
    cmd = Command::SCROLL_INTERVAL;
  else if (strcmp_P(command, PSTR("init_speed")) == 0)
    cmd = Command::INIT_SPEED;
  else if (strcmp_P(command, PSTR("base_speed")) == 0)
    cmd = Command::BASE_SPEED;
  else if (strcmp_P(command, PSTR("accel_duration")) == 0)
    cmd = Command::ACCEL_DURATION;
  else
    return EventHandlerResult::ABORT;

  if (::Focus.isEOL()) {
    // If there is no argument given, we send back the current value of the
    // setting that was requested.
    uint16_t val;
    switch (cmd) {
    case Command::SCROLL_INTERVAL:
      val = ::MouseKeys.getScrollInterval();
      break;
    case Command::INIT_SPEED:
      val = ::MouseKeys.getCursorInitSpeed();
      break;
    case Command::BASE_SPEED:
      val = ::MouseKeys.getCursorBaseSpeed();
      break;
    case Command::ACCEL_DURATION:
      val = ::MouseKeys.getCursorAccelDuration();
      break;
    default:
      return EventHandlerResult::ABORT;
    }
    ::Focus.send(val);
    return EventHandlerResult::EVENT_CONSUMED;
  } else {
    // If there is an argument, we read it, then pass it to the corresponding
    // setter method of MouseKeys.
    uint16_t arg;
    ::Focus.read(arg);

    switch (cmd) {
    case Command::SCROLL_INTERVAL:
      ::MouseKeys.setScrollInterval(arg);
      break;
    case Command::INIT_SPEED:
      ::MouseKeys.setCursorInitSpeed(arg);
      break;
    case Command::BASE_SPEED:
      ::MouseKeys.setCursorBaseSpeed(arg);
      break;
    case Command::ACCEL_DURATION:
      ::MouseKeys.setCursorAccelDuration(arg);
      break;
    }
  }

  // Update settings stored in EEPROM, and indicate that this Focus event has
  // been handled successfully.
  Runtime.storage().put(settings_addr_, ::MouseKeys.settings_);
  Runtime.storage().commit();
  return EventHandlerResult::EVENT_CONSUMED;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::MouseKeysConfig MouseKeysConfig;

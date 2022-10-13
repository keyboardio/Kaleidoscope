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

  // If the EEPROM is empty, store the default settings.
  if (Runtime.storage().isSliceUninitialized(settings_addr_, sizeof(MouseKeys::Settings))) {
    Runtime.storage().put(settings_addr_, ::MouseKeys.settings_);
    Runtime.storage().commit();
  }

  Runtime.storage().get(settings_addr_, ::MouseKeys.settings_);

  // We need to set the grid size explicitly, because behind the scenes, that's
  // stored in MouseWrapper.
  ::MouseKeys.setWarpGridSize(::MouseKeys.settings_.warp_grid_size);

  return EventHandlerResult::OK;
}

// -----------------------------------------------------------------------------
EventHandlerResult MouseKeysConfig::onFocusEvent(const char *input) {
  enum Command : uint8_t {
    SCROLL_INTERVAL,
    INIT_SPEED,
    BASE_SPEED,
    ACCEL_DURATION,
    WARP_GRID_SIZE,
  } cmd;
  const char *cmd_scroll_interval = PSTR("mousekeys.scroll_interval");
  const char *cmd_initial_speed   = PSTR("mousekeys.init_speed");
  const char *cmd_base_speed      = PSTR("mousekeys.base_speed");
  const char *cmd_accel_duration  = PSTR("mousekeys.accel_duration");
  const char *cmd_warp_grid_size  = PSTR("mousekeys.warp_grid_size");

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(
      cmd_scroll_interval,
      cmd_initial_speed,
      cmd_base_speed,
      cmd_accel_duration,
      cmd_warp_grid_size);

  if (::Focus.inputMatchesCommand(input, cmd_scroll_interval))
    cmd = Command::SCROLL_INTERVAL;
  else if (::Focus.inputMatchesCommand(input, cmd_initial_speed))
    cmd = Command::INIT_SPEED;
  else if (::Focus.inputMatchesCommand(input, cmd_base_speed))
    cmd = Command::BASE_SPEED;
  else if (::Focus.inputMatchesCommand(input, cmd_accel_duration))
    cmd = Command::ACCEL_DURATION;
  else if (::Focus.inputMatchesCommand(input, cmd_warp_grid_size))
    cmd = Command::WARP_GRID_SIZE;
  else
    // allow other plugins to process this event.
    return EventHandlerResult::OK;

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
    case Command::WARP_GRID_SIZE:
      val = ::MouseKeys.getWarpGridSize();
      break;
    default:
      // if a valid command is issued but there is no 0-arg handler for it,
      // we stop processing the event.
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
    case Command::WARP_GRID_SIZE:
      ::MouseKeys.setWarpGridSize(arg);
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

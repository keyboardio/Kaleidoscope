/* -*- mode: c++ -*-
 * Kaleidoscope-LongPress -- Provide different key strokes on long press
 * Copyright (C) 2024  Keyboard.io, Inc
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

#include "kaleidoscope/plugin/LongPress.h"  // IWYU pragma: associated

#include <Arduino.h>                       // for PSTR
#include <Kaleidoscope-EEPROM-Settings.h>  // for EEPROMSettings
#include <Kaleidoscope-FocusSerial.h>      // for Focus, FocusSerial
#include <stdint.h>                        // for uint8_t, uint16_t

#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for VirtualProps::Storage, Base<>::Storage
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK

namespace kaleidoscope {
namespace plugin {

// =============================================================================
// LongPress configurator

EventHandlerResult LongPressConfig::onSetup() {
  ::EEPROMSettings.requestSliceAndLoadData(&settings_base_, &::LongPress.settings_);
  return EventHandlerResult::OK;
}

void LongPressConfig::disableLongPressIfUnconfigured() {
  if (Runtime.storage().isSliceUninitialized(settings_base_, sizeof(LongPress::settings_)))
    ::LongPress.disable();
}

EventHandlerResult LongPressConfig::onFocusEvent(const char *input) {
  enum {
    ENABLED,
    TIMEOUT,
    AUTOSHIFT,
  } subCommand;

  const char *cmd_enabled   = PSTR("longpress.enabled");
  const char *cmd_timeout   = PSTR("longpress.timeout");
  const char *cmd_autoshift = PSTR("longpress.autoshift");

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd_enabled, cmd_timeout, cmd_autoshift);

  if (::Focus.inputMatchesCommand(input, cmd_enabled))
    subCommand = ENABLED;
  else if (::Focus.inputMatchesCommand(input, cmd_timeout))
    subCommand = TIMEOUT;
  else if (::Focus.inputMatchesCommand(input, cmd_autoshift))
    subCommand = AUTOSHIFT;
  else
    return EventHandlerResult::OK;

  switch (subCommand) {
  case ENABLED:
    if (::Focus.isEOL()) {
      ::Focus.send(::LongPress.enabled());
    } else {
      uint8_t v;
      ::Focus.read(v);
      // if (::Focus.readUint8() != 0) {
      if (v != 0) {
        ::LongPress.enable();
      } else {
        ::LongPress.disable();
      }
    }
    break;

  case TIMEOUT:
    if (::Focus.isEOL()) {
      ::Focus.send(::LongPress.timeout());
    } else {
      uint16_t t;
      ::Focus.read(t);
      // auto t = ::Focus.readUint16();
      ::LongPress.setTimeout(t);
    }
    break;

  case AUTOSHIFT:
    if (::Focus.isEOL()) {
      ::Focus.send(uint8_t(::LongPress.enabledAutoShiftCategories()));
    } else {
      uint8_t v;
      ::Focus.read(v);
      auto autoshift = longpress::AutoShiftCategories(v);
      // auto autoshift = longpress::AutoShiftCategories(::Focus.readUint8());
      ::LongPress.setAutoshiftEnabled(autoshift);
    }
    break;
  }

  Runtime.storage().put(settings_base_, ::LongPress.settings_);
  Runtime.storage().commit();
  return EventHandlerResult::EVENT_CONSUMED;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::LongPressConfig LongPressConfig;

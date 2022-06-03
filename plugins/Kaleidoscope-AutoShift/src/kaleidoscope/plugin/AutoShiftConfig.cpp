/* -*- mode: c++ -*-
 * Kaleidoscope-AutoShift -- Automatic shift on long press
 * Copyright (C) 2021  Keyboard.io, Inc
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

#include "kaleidoscope/plugin/AutoShift.h"  // IWYU pragma: associated

#include <Arduino.h>                       // for PSTR, strcmp_P, strncmp_P
#include <Kaleidoscope-EEPROM-Settings.h>  // for EEPROMSettings
#include <Kaleidoscope-FocusSerial.h>      // for Focus, FocusSerial
#include <stdint.h>                        // for uint8_t, uint16_t

#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for VirtualProps::Storage, Base<>::Storage
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK

namespace kaleidoscope {
namespace plugin {

// =============================================================================
// AutoShift configurator

EventHandlerResult AutoShiftConfig::onSetup() {
  settings_base_ = ::EEPROMSettings.requestSlice(sizeof(AutoShift::Settings));

  if (Runtime.storage().isSliceUninitialized(
        settings_base_,
        sizeof(AutoShift::Settings))) {
    // If our slice is uninitialized, set sensible defaults.
    Runtime.storage().put(settings_base_, ::AutoShift.settings_);
    Runtime.storage().commit();
  }

  Runtime.storage().get(settings_base_, ::AutoShift.settings_);
  return EventHandlerResult::OK;
}

EventHandlerResult AutoShiftConfig::onFocusEvent(const char *command) {
  enum {
    ENABLED,
    TIMEOUT,
    CATEGORIES,
  } subCommand;

  if (::Focus.handleHelp(command, PSTR("autoshift.enabled\n"
                                       "autoshift.timeout\n"
                                       "autoshift.categories")))
    return EventHandlerResult::OK;

  if (strncmp_P(command, PSTR("autoshift."), 10) != 0)
    return EventHandlerResult::OK;
  if (strcmp_P(command + 10, PSTR("enabled")) == 0)
    subCommand = ENABLED;
  else if (strcmp_P(command + 10, PSTR("timeout")) == 0)
    subCommand = TIMEOUT;
  else if (strcmp_P(command + 10, PSTR("categories")) == 0)
    subCommand = CATEGORIES;
  else
    return EventHandlerResult::OK;

  switch (subCommand) {
  case ENABLED:
    if (::Focus.isEOL()) {
      ::Focus.send(::AutoShift.enabled());
    } else {
      uint8_t v;
      ::Focus.read(v);
      // if (::Focus.readUint8() != 0) {
      if (v != 0) {
        ::AutoShift.enable();
      } else {
        ::AutoShift.disable();
      }
    }
    break;

  case TIMEOUT:
    if (::Focus.isEOL()) {
      ::Focus.send(::AutoShift.timeout());
    } else {
      uint16_t t;
      ::Focus.read(t);
      // auto t = ::Focus.readUint16();
      ::AutoShift.setTimeout(t);
    }
    break;

  case CATEGORIES:
    if (::Focus.isEOL()) {
      ::Focus.send(uint8_t(::AutoShift.enabledCategories()));
    } else {
      uint8_t v;
      ::Focus.read(v);
      auto categories = AutoShift::Categories(v);
      // auto categories = AutoShift::Categories(::Focus.readUint8());
      ::AutoShift.setEnabled(categories);
    }
    break;
  }

  Runtime.storage().put(settings_base_, ::AutoShift.settings_);
  Runtime.storage().commit();
  return EventHandlerResult::EVENT_CONSUMED;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::AutoShiftConfig AutoShiftConfig;

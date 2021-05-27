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

#include "kaleidoscope/plugin/AutoShift.h"

#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-FocusSerial.h>

#include "kaleidoscope/Runtime.h"

namespace kaleidoscope {
namespace plugin {

// =============================================================================
// AutoShift configurator

uint16_t AutoShiftConfig::settings_base_;

EventHandlerResult AutoShiftConfig::onSetup() {
  settings_base_ = ::EEPROMSettings.requestSlice(sizeof(AutoShift::settings_));
  uint32_t checker;

  Runtime.storage().get(settings_base_, checker);

  // Check if we have an empty eeprom...
  if (checker == 0xffffffff) {
    // ...if the eeprom was empty, store the default settings.
    Runtime.storage().put(settings_base_, AutoShift::settings_);
    Runtime.storage().commit();
  }

  Runtime.storage().get(settings_base_, AutoShift::settings_);
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

  Runtime.storage().put(settings_base_, AutoShift::settings_);
  Runtime.storage().commit();
  return EventHandlerResult::EVENT_CONSUMED;
}

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::AutoShiftConfig AutoShiftConfig;

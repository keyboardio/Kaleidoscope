/* -*- mode: c++ -*-
 * kaleidoscope::plugin::PersistentLEDMode -- Persist the current LED mode to Storage
 * Copyright (C) 2019-2022  Keyboard.io, Inc.
 * Copyright (C) 2019  Dygma, Inc.
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

#include "kaleidoscope/plugin/PersistentLEDMode.h"

#include <Kaleidoscope-EEPROM-Settings.h>  // for EEPROMSettings
#include <Kaleidoscope-FocusSerial.h>      // for Focus
#include <stdint.h>                        // for uint8_t, uint16_t

#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for VirtualProps::Storage, Base<>::Storage
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/plugin/LEDControl.h"     // for LEDControl

namespace kaleidoscope {
namespace plugin {

uint16_t PersistentLEDMode::settings_base_;
struct PersistentLEDMode::settings PersistentLEDMode::settings_;

EventHandlerResult PersistentLEDMode::onSetup() {
  settings_base_ = ::EEPROMSettings.requestSlice(sizeof(settings_));

  Runtime.storage().get(settings_base_, settings_);

  // If our slice is uninitialized, then return early, without touching the
  // current mode. We want auto_save by default, but because EEPROM is
  // uninitialized (0xff), that'll be set anyway, so we don't need to. This
  // saves us a storage commit.
  if (Runtime.storage().isSliceUninitialized(settings_base_, sizeof(settings_)))
    return EventHandlerResult::OK;

  ::LEDControl.set_mode(settings_.default_mode_index);

  return EventHandlerResult::OK;
}

EventHandlerResult PersistentLEDMode::onLEDModeChange() {
  if (!settings_.auto_save)
    return EventHandlerResult::OK;

  if (settings_.default_mode_index == ::LEDControl.get_mode_index())
    return EventHandlerResult::OK;

  settings_.default_mode_index = ::LEDControl.get_mode_index();
  Runtime.storage().put(settings_base_, settings_);
  Runtime.storage().commit();

  return EventHandlerResult::OK;
}

EventHandlerResult PersistentLEDMode::onFocusEvent(const char *command) {
  enum {
    AUTO_SAVE,
    DEFAULT_MODE,
  } sub_command;

  if (::Focus.handleHelp(command, PSTR("led_mode.default\nled_mode.auto_save")))
    return EventHandlerResult::OK;

  if (strncmp_P(command, PSTR("led_mode."), 9) != 0)
    return EventHandlerResult::OK;

  if (strcmp_P(command + 9, PSTR("default")) == 0)
    sub_command = DEFAULT_MODE;
  else if (strcmp_P(command + 9, PSTR("auto_save")) == 0)
    sub_command = AUTO_SAVE;
  else
    return EventHandlerResult::OK;

  switch (sub_command) {
  case DEFAULT_MODE: {
    if (::Focus.isEOL()) {
      ::Focus.send(settings_.default_mode_index);
    } else {
      uint8_t idx;
      ::Focus.read(idx);
      settings_.default_mode_index = idx;
      ::LEDControl.set_mode(idx);
      Runtime.storage().put(settings_base_, settings_);
      Runtime.storage().commit();
    }
    break;
  }

  case AUTO_SAVE: {
    if (::Focus.isEOL()) {
      ::Focus.send(settings_.auto_save);
    } else {
      uint8_t v;
      ::Focus.read(v);
      setAutoSave(v != 0);
    }
    break;
  }
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

void PersistentLEDMode::setAutoSave(bool state) {
  settings_.auto_save = state;
  Runtime.storage().put(settings_base_, settings_);
  Runtime.storage().commit();
}

EventHandlerResult PersistentLEDMode::onNameQuery() {
  return ::Focus.sendName(F("PersistentLEDMode"));
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::PersistentLEDMode PersistentLEDMode;

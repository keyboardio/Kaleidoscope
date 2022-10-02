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

#include <Arduino.h>                       // for PSTR, F, __FlashStringHelper
#include <Kaleidoscope-EEPROM-Settings.h>  // for EEPROMSettings
#include <Kaleidoscope-FocusSerial.h>      // for Focus, FocusSerial
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
  // current mode.
  if (Runtime.storage().isSliceUninitialized(settings_base_, sizeof(settings_)))
    return EventHandlerResult::OK;

  ::LEDControl.set_mode(settings_.default_mode_index);

  return EventHandlerResult::OK;
}

EventHandlerResult PersistentLEDMode::onLEDModeChange() {
  if (settings_.default_mode_index == ::LEDControl.get_mode_index())
    return EventHandlerResult::OK;

  settings_.default_mode_index = ::LEDControl.get_mode_index();
  Runtime.storage().put(settings_base_, settings_);
  Runtime.storage().commit();

  return EventHandlerResult::OK;
}

EventHandlerResult PersistentLEDMode::onNameQuery() {
  return ::Focus.sendName(F("PersistentLEDMode"));
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::PersistentLEDMode PersistentLEDMode;

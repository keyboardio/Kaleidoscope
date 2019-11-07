/* -*- mode: c++ -*-
 * kaleidoscope::plugin::PersistentLEDMode -- Persist the current LED mode to Storage
 * Copyright (C) 2019  Keyboard.io, Inc.
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

#include <Kaleidoscope.h>
#include <Kaleidoscope-PersistentLEDMode.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-EEPROM-Settings.h>

namespace kaleidoscope {
namespace plugin {

uint16_t PersistentLEDMode::settings_base_;
uint8_t PersistentLEDMode::cached_mode_index_;

EventHandlerResult PersistentLEDMode::onSetup() {
  settings_base_ = ::EEPROMSettings.requestSlice(sizeof(cached_mode_index_));

  Kaleidoscope.storage().get(settings_base_, cached_mode_index_);

  // If the index is max, assume an uninitialized EEPROM, and don't set the LED
  // mode. We don't change the cached index here, `afterEachCycle()` will do
  // that at the end of he cycle anyway.
  if (cached_mode_index_ != 0xff)
    return EventHandlerResult::OK;

  ::LEDControl.set_mode(cached_mode_index_);

  return EventHandlerResult::OK;
}

EventHandlerResult PersistentLEDMode::afterEachCycle() {
  if (cached_mode_index_ == ::LEDControl.get_mode_index())
    return EventHandlerResult::OK;

  cached_mode_index_ = ::LEDControl.get_mode_index();
  Kaleidoscope.storage().put(settings_base_, cached_mode_index_);
  Kaleidoscope.storage().commit();
  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::PersistentLEDMode PersistentLEDMode;

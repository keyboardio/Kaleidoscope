/* -*- mode: c++ -*-
 * Kaleidoscope-HostOS -- Host OS detection and tracking for Kaleidoscope
 * Copyright (C) 2016, 2017, 2018  Keyboard.io, Inc
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

#include "kaleidoscope/plugin/HostOS.h"

#include <Kaleidoscope-EEPROM-Settings.h>  // for EEPROMSettings

#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for VirtualProps::Storage, Base<>::Storage
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK

namespace kaleidoscope {
namespace plugin {

EventHandlerResult HostOS::onSetup() {
  if (is_configured_)
    return EventHandlerResult::OK;

  eeprom_slice_ = ::EEPROMSettings.requestSlice(sizeof(os_));

  is_configured_ = true;

  if (os_ != hostos::UNKNOWN) {
    return EventHandlerResult::OK;
  }

  os_ = (hostos::Type)Runtime.storage().read(eeprom_slice_);

  return EventHandlerResult::OK;
}

void HostOS::os(hostos::Type new_os) {
  os_ = new_os;
  Runtime.storage().update(eeprom_slice_, os_);
  Runtime.storage().commit();
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::HostOS HostOS;

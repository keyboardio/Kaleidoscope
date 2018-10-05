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

#include <Kaleidoscope/HostOS-Base.h>
#include <Kaleidoscope-EEPROM-Settings.h>

#include <EEPROM.h>

namespace kaleidoscope {
namespace hostos {

EventHandlerResult Base::onSetup(void) {
  if (is_configured_)
    return EventHandlerResult::OK;

  eeprom_slice_ = ::EEPROMSettings.requestSlice(sizeof(os_));

  is_configured_ = true;

  if (os_ != AUTO) {
    return EventHandlerResult::OK;
  }

  if ((os_ = (Type)EEPROM.read(eeprom_slice_)) != AUTO)
    return EventHandlerResult::OK;

  autoDetect();

  return EventHandlerResult::OK;
}

Type Base::os(void) {
  if (os_ == AUTO)
    autoDetect();

  return os_;
}

void Base::os(Type new_os) {
  os_ = new_os;
  EEPROM.update(eeprom_slice_, os_);
}

}
}

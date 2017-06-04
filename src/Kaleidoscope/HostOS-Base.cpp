/* -*- mode: c++ -*-
 * Kaleidoscope-HostOS -- Host OS detection and tracking for Kaleidoscope
 * Copyright (C) 2016, 2017  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Kaleidoscope/HostOS-Base.h>
#include <Kaleidoscope-EEPROM-Settings.h>

#include <EEPROM.h>

namespace kaleidoscope {
namespace hostos {

void Base::begin(void) {
  if (is_configured_)
    return;

  eeprom_slice_ = ::EEPROMSettings.requestSlice(sizeof(os_));

  is_configured_ = true;

  if (os_ != AUTO) {
    return;
  }

  if ((os_ = (Type)EEPROM.read(eeprom_slice_)) != AUTO)
    return;

  autoDetect();
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

bool Base::focusHook(const char *command) {
  if (strcmp_P(command, PSTR("hostos.type")) != 0)
    return false;

  if (Serial.peek() == '\n') {
    Serial.println(os_);
  } else {
    uint8_t new_os = Serial.parseInt();
    os((Type) new_os);
  }

  Serial.read();
  return true;
}

}
}

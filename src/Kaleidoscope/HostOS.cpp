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

#include <Kaleidoscope/HostOS.h>

#include <EEPROM.h>

#define EEPROM_HOSTOS_TYPE_LOCATION 2

namespace KaleidoscopePlugins {
  namespace HostOS {
    void
    Base::begin (void) {
      if (isConfigured)
        return;

      isConfigured = true;

      if (osType != AUTO) {
        return;
      }

      if ((osType = (Type)EEPROM.read (EEPROM_HOSTOS_TYPE_LOCATION)) != AUTO)
        return;

      autoDetect ();
    }

    HostOS::Type
    Base::os (void) {
      if (osType == AUTO)
        autoDetect ();

      return osType;
    }

    void
    Base::os (HostOS::Type osType_) {
      osType = osType_;
      EEPROM.update (EEPROM_HOSTOS_TYPE_LOCATION, osType);
    }
  };
};

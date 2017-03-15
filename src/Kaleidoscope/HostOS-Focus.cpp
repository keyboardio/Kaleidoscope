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
#include <Kaleidoscope/HostOS-Focus.h>

namespace KaleidoscopePlugins {
  namespace HostOS {
    bool
    Focus (const char *command) {
      enum {
        GET,
        SET,
      } subCommand;

      if (strncmp_P (command, PSTR ("hostos."), 7) != 0)
        return false;
      if (strcmp_P (command + 7, PSTR ("type")) == 0)
        subCommand = GET;
      else if (strcmp_P (command + 7, PSTR ("set")) == 0)
        subCommand = SET;
      else
        return false;

      switch (subCommand) {
      case GET:
        Serial.println (::HostOS.os ());
        break;
      case SET:
        uint8_t os = Serial.parseInt ();
        ::HostOS.os ((KaleidoscopePlugins::HostOS::Type) os);
        break;
      }

      Serial.read ();
      return true;
    }
  }
}

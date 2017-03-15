/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Settings -- Basic EEPROM settings plugin for Kaleidoscope.
 * Copyright (C) 2017  Gergely Nagy
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

#include <Kaleidoscope-EEPROM-Settings.h>
#include "crc.h"

namespace FocusHooks {
  bool settings (const char *command) {
    enum {
      ISVALID,
      GETVERSION,
      CRC,
    } subCommand;

    if (strncmp_P (command, PSTR ("settings."), 9) != 0)
      return false;

    if (strcmp_P (command + 9, PSTR ("valid?")) == 0)
      subCommand = ISVALID;
    else if (strcmp_P (command + 9, PSTR ("version")) == 0)
      subCommand = GETVERSION;
    else if (strcmp_P (command + 9, PSTR ("crc")) == 0)
      subCommand = CRC;
    else
      return false;

    switch (subCommand) {
    case ISVALID:
      Serial.println (EEPROMSettings.isValid () ? F("yes") : F("no"));
      break;
    case GETVERSION:
      Serial.println (EEPROMSettings.version ());
    case CRC:
      Serial.print (::CRC.crc, HEX);
      Serial.print (F("/"));
      Serial.println (EEPROMSettings.crc (), HEX);
      break;
    }

    Serial.read ();

    return true;
  }
};

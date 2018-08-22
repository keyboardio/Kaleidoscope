/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Settings -- Basic EEPROM settings plugin for Kaleidoscope.
 * Copyright (C) 2017  Gergely Nagy
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

#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-Focus.h>
#include "crc.h"

namespace kaleidoscope {
namespace eeprom_settings {

bool settingsFocusHook(const char *command) {
  enum {
    ISVALID,
    GETVERSION,
    CRC,
  } sub_command;

  if (strncmp_P(command, PSTR("settings."), 9) != 0)
    return false;

  if (strcmp_P(command + 9, PSTR("valid?")) == 0)
    sub_command = ISVALID;
  else if (strcmp_P(command + 9, PSTR("version")) == 0)
    sub_command = GETVERSION;
  else if (strcmp_P(command + 9, PSTR("crc")) == 0)
    sub_command = CRC;
  else
    return false;

  switch (sub_command) {
  case ISVALID:
    ::Focus.printBool(::EEPROMSettings.isValid());
    Serial.println();
    break;
  case GETVERSION:
    Serial.println(::EEPROMSettings.version());
    break;
  case CRC:
    Serial.print(::CRC.crc, HEX);
    Serial.print(F("/"));
    Serial.println(::EEPROMSettings.crc(), HEX);
    break;
  }

  return true;
}

bool eepromFocusHook(const char *command) {
  enum {
    CONTENTS,
    FREE,
  } sub_command;

  if (strcmp_P(command, PSTR("eeprom.contents")) == 0)
    sub_command = CONTENTS;
  else if (strcmp_P(command, PSTR("eeprom.free")) == 0)
    sub_command = FREE;
  else
    return false;

  switch (sub_command) {
  case CONTENTS: {
    if (Serial.peek() == '\n') {
      for (uint16_t i = 0; i < EEPROM.length(); i++) {
        uint8_t d = EEPROM[i];
        ::Focus.printNumber(d);
        ::Focus.printSpace();
      }
      Serial.println();
    } else {
      for (uint16_t i = 0; i < EEPROM.length() && Serial.peek() != '\n'; i++) {
        uint8_t d = Serial.parseInt();
        EEPROM.update(i, d);
      }
    }

    break;
  }
  case FREE:
    Serial.println(EEPROM.length() - ::EEPROMSettings.used());
    break;
  }

  return true;
}

}

}

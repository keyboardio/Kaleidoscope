/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Settings -- Basic EEPROM settings plugin for Kaleidoscope.
 * Copyright (C) 2017-2018  Keyboard.io, Inc
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
#include <Kaleidoscope-FocusSerial.h>
#include "crc.h"

namespace kaleidoscope {
namespace eeprom {

EventHandlerResult FocusSettingsCommand::onFocusEvent(const char *command) {
  enum {
    DEFAULT_LAYER,
    IS_VALID,
    GET_VERSION,
    CRC,
  } sub_command;

  if (::Focus.handleHelp(command, PSTR("settings.defaultLayer\nsettings.valid?\nsettings.version\nsettings.crc")))
    return EventHandlerResult::OK;

  if (strncmp_P(command, PSTR("settings."), 9) != 0)
    return EventHandlerResult::OK;

  if (strcmp_P(command + 9, PSTR("defaultLayer")) == 0)
    sub_command = DEFAULT_LAYER;
  else if (strcmp_P(command + 9, PSTR("valid?")) == 0)
    sub_command = IS_VALID;
  else if (strcmp_P(command + 9, PSTR("version")) == 0)
    sub_command = GET_VERSION;
  else if (strcmp_P(command + 9, PSTR("crc")) == 0)
    sub_command = CRC;
  else
    return EventHandlerResult::OK;

  switch (sub_command) {
  case DEFAULT_LAYER: {
    if (Serial.peek() == '\n') {
      Serial.println(::EEPROMSettings.default_layer());
    } else {
      ::EEPROMSettings.default_layer(Serial.parseInt());
    }
    break;
  }
  case IS_VALID:
    ::Focus.printBool(::EEPROMSettings.isValid());
    Serial.println();
    break;
  case GET_VERSION:
    Serial.println(::EEPROMSettings.version());
    break;
  case CRC:
    Serial.print(::CRC.crc, HEX);
    Serial.print(F("/"));
    Serial.println(::EEPROMSettings.crc(), HEX);
    break;
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult FocusEEPROMCommand::onFocusEvent(const char *command) {
  enum {
    CONTENTS,
    FREE,
  } sub_command;

  if (::Focus.handleHelp(command, PSTR("eeprom.contents\neeprom.free")))
    return EventHandlerResult::OK;

  if (strcmp_P(command, PSTR("eeprom.contents")) == 0)
    sub_command = CONTENTS;
  else if (strcmp_P(command, PSTR("eeprom.free")) == 0)
    sub_command = FREE;
  else
    return EventHandlerResult::OK;

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

  return EventHandlerResult::EVENT_CONSUMED;
}

}
}

kaleidoscope::eeprom::FocusSettingsCommand FocusSettingsCommand;
kaleidoscope::eeprom::FocusEEPROMCommand FocusEEPROMCommand;

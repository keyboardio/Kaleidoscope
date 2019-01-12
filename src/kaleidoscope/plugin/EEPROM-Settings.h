/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Settings -- Basic EEPROM settings plugin for Kaleidoscope.
 * Copyright (C) 2017, 2018  Keyboard.io, Inc
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

#pragma once

#include <Kaleidoscope.h>
#include <EEPROM.h>

namespace kaleidoscope {
namespace plugin {
class EEPROMSettings : public kaleidoscope::Plugin {
 public:
  EEPROMSettings(void) {}

  EventHandlerResult onSetup();
  EventHandlerResult beforeEachCycle();

  static void update(void);
  static bool isValid(void);
  static void invalidate(void);
  static uint8_t version(void);
  static void version(uint8_t ver);

  static uint16_t requestSlice(uint16_t size);
  static void seal(void);
  static uint16_t crc(void);
  static uint16_t used(void);

  static uint8_t default_layer(uint8_t layer);
  static uint8_t default_layer() {
    return settings_.default_layer;
  }

 private:
  static uint16_t next_start_;
  static bool is_valid_;
  static bool sealed_;

  static struct settings {
    uint8_t default_layer;
    uint8_t version;
    uint16_t crc;
  } settings_;
};

class FocusSettingsCommand : public kaleidoscope::Plugin {
 public:
  FocusSettingsCommand() {}

  EventHandlerResult onFocusEvent(const char *command);
};

class FocusEEPROMCommand : public kaleidoscope::Plugin {
 public:
  FocusEEPROMCommand() {}

  EventHandlerResult onFocusEvent(const char *command);
};
}
}

extern kaleidoscope::plugin::EEPROMSettings EEPROMSettings;
extern kaleidoscope::plugin::FocusSettingsCommand FocusSettingsCommand;
extern kaleidoscope::plugin::FocusEEPROMCommand FocusEEPROMCommand;

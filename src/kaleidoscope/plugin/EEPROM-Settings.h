/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Settings -- Basic EEPROM settings plugin for Kaleidoscope.
 * Copyright (C) 2017, 2018, 2019  Keyboard.io, Inc
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

#define _DEPRECATED_MESSAGE_EEPROMSETTINGS_VERSION_SET            \
  "The EEPROMSettings.version(uint8_t version) method has been deprecated,\n" \
  "and is a no-op now. Please see the NEWS file for more information."

namespace kaleidoscope {
namespace plugin {
class EEPROMSettings : public kaleidoscope::Plugin {
 public:
  EEPROMSettings(void) {}

  EventHandlerResult onSetup();
  EventHandlerResult beforeEachCycle();

  /* EEPROM is filled with 0xff when uninitialized, so a version with that value
   * means we do not have an EEPROM version defined yet. */
  static constexpr uint8_t VERSION_UNDEFINED = 0xff;
  /* A version set to zero is likely some kind of corruption, we do not normally
   * clear the byte. */
  static constexpr uint8_t VERSION_IMPOSSIBLE_ZERO = 0x00;
  /* Our current version. Whenever we change the layout of the settings, this
   * needs to be increased too. If the version stored in EEPROM does not match
   * this version, EEPROM use should be considered unsafe, and plugins should
   * fall back to not using it. */
  static constexpr uint8_t VERSION_CURRENT = 0x01;

  static void update(void);
  static bool isValid(void);
  static void invalidate(void);
  static uint8_t version(void) {
    return settings_.version;
  }
  static void version(uint8_t) DEPRECATED(EEPROMSETTINGS_VERSION_SET) {}

  static uint16_t requestSlice(uint16_t size);
  static void seal(void);
  static uint16_t crc(void);
  static uint16_t used(void);

  static uint8_t default_layer(uint8_t layer);
  static uint8_t default_layer() {
    return settings_.default_layer;
  }
  static void ignoreHardcodedLayers(bool value);
  static bool ignoreHardcodedLayers() {
    return settings_.ignore_hardcoded_layers;
  }

 private:
  static constexpr uint8_t IGNORE_HARDCODED_LAYER = 0x7e;
  static uint16_t next_start_;
  static bool is_valid_;
  static bool sealed_;

  static struct settings {
    uint8_t default_layer: 7;
    bool ignore_hardcoded_layers: 1;
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

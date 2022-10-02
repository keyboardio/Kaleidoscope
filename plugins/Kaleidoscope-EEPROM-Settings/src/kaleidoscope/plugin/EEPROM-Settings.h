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

#include <stdint.h>  // for uint8_t, uint16_t

#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin

namespace kaleidoscope {
namespace plugin {

class EEPROMSettings : public kaleidoscope::Plugin {
 private:
  struct Settings {
    uint8_t default_layer : 7;
    bool ignore_hardcoded_layers : 1;
    uint8_t version;
    uint16_t crc;
  };

 public:
  EventHandlerResult onSetup();
  EventHandlerResult beforeEachCycle();

  /* EEPROM is filled with 0xff when uninitialized, and we use that in a few
   * places. */
  static constexpr uint8_t EEPROM_UNINITIALIZED_BYTE = 0xff;
  /* EEPROM is filled with 0xff when uninitialized, so a version with that value
   * means we do not have an EEPROM version defined yet. */
  static constexpr uint8_t VERSION_UNDEFINED = EEPROM_UNINITIALIZED_BYTE;
  /* A version set to zero is likely some kind of corruption, we do not normally
   * clear the byte. */
  static constexpr uint8_t VERSION_IMPOSSIBLE_ZERO = 0x00;
  /* Our current version. Whenever we change the layout of the settings, this
   * needs to be increased too. If the version stored in EEPROM does not match
   * this version, EEPROM use should be considered unsafe, and plugins should
   * fall back to not using it. */
  static constexpr uint8_t VERSION_CURRENT = 0x01;

  void update();
  bool isValid();
  void invalidate();
  uint8_t version() {
    return settings_.version;
  }

  uint16_t requestSlice(uint16_t size);
  void seal();
  uint16_t crc();
  uint16_t used();

  uint8_t default_layer(uint8_t layer);
  uint8_t default_layer() {
    return settings_.default_layer;
  }
  void ignoreHardcodedLayers(bool value);
  bool ignoreHardcodedLayers() {
    return settings_.ignore_hardcoded_layers;
  }

 private:
  static constexpr uint8_t IGNORE_HARDCODED_LAYER = 0x7e;

  uint16_t next_start_ = sizeof(EEPROMSettings::Settings);
  bool is_valid_;
  bool sealed_;

  Settings settings_;
};

class FocusSettingsCommand : public kaleidoscope::Plugin {
 public:
  EventHandlerResult onFocusEvent(const char *input);
};

class FocusEEPROMCommand : public kaleidoscope::Plugin {
 public:
  EventHandlerResult onFocusEvent(const char *input);
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::EEPROMSettings EEPROMSettings;
extern kaleidoscope::plugin::FocusSettingsCommand FocusSettingsCommand;
extern kaleidoscope::plugin::FocusEEPROMCommand FocusEEPROMCommand;

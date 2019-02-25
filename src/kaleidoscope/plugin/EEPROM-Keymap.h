/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Keymap -- EEPROM-based keymap support.
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
#include <Kaleidoscope-EEPROM-Settings.h>

#define _DEPRECATED_MESSAGE_EEPROM_KEYMAP_SETUP_MODE                       \
  "The `mode` argument of EEPROMKeymap.setup() is deprecated and is not\n" \
  "used anymore. You can remove it safely."

namespace kaleidoscope {
namespace plugin {
class EEPROMKeymap : public kaleidoscope::Plugin {
 public:
  enum class Mode {
    CUSTOM,
    EXTEND
  };

  EEPROMKeymap(void) {}

  EventHandlerResult onSetup();
  EventHandlerResult onFocusEvent(const char *command);

  static void setup(uint8_t max);
  static void setup(uint8_t max, Mode mode) DEPRECATED(EEPROM_KEYMAP_SETUP_MODE) {
    setup(max);
  }

  static void max_layers(uint8_t max);

  static uint16_t keymap_base(void);

  static Key getKey(uint8_t layer, KeyAddr key_addr);
  KS_ROW_COL_FUNC static Key getKey(uint8_t layer, byte row, byte col) {
    return getKey(layer, KeyAddr(row, col));
  }
  static Key getKeyExtended(uint8_t layer, KeyAddr key_addr);
  KS_ROW_COL_FUNC static Key getKeyExtended(uint8_t layer, byte row, byte col) {
    return getKeyExtended(layer, KeyAddr(row, col));
  }

  static void updateKey(uint16_t base_pos, Key key);

 private:
  static uint16_t keymap_base_;
  static uint8_t max_layers_;
  static uint8_t progmem_layers_;

  static Key parseKey(void);
  static void printKey(Key key);
  static void dumpKeymap(uint8_t layers, Key(*getkey)(uint8_t, KeyAddr));
  KS_ROW_COL_FUNC static void dumpKeymap(uint8_t layers, Key(*getkey)(uint8_t, byte, byte));
};
}
}

extern kaleidoscope::plugin::EEPROMKeymap EEPROMKeymap;

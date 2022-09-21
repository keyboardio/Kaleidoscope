/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Keymap -- EEPROM-based keymap support.
 * Copyright (C) 2017, 2018, 2019, 2021  Keyboard.io, Inc
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

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/plugin.h"                // for Plugin

namespace kaleidoscope {
namespace plugin {
class EEPROMKeymap : public kaleidoscope::Plugin {
 public:
  enum class Mode {
    CUSTOM,
    EXTEND
  };

  EventHandlerResult onSetup();
  EventHandlerResult onNameQuery();
  EventHandlerResult onFocusEvent(const char *input);

  static void setup(uint8_t max);

  static void max_layers(uint8_t max);

  static uint16_t keymap_base();

  static Key getKey(uint8_t layer, KeyAddr key_addr);
  static Key getKeyExtended(uint8_t layer, KeyAddr key_addr);

  static void updateKey(uint16_t base_pos, Key key);

 private:
  static uint16_t keymap_base_;
  static uint8_t max_layers_;
  static uint8_t progmem_layers_;

  static Key parseKey();
  static void printKey(Key key);
  static void dumpKeymap(uint8_t layers, Key (*getkey)(uint8_t, KeyAddr));
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::EEPROMKeymap EEPROMKeymap;

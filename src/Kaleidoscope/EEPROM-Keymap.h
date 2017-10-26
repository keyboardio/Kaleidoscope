/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Keymap -- EEPROM-based keymap support.
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

#pragma once

#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Settings.h>

namespace kaleidoscope {
class EEPROMKeymap : public KaleidoscopePlugin {
 public:
  EEPROMKeymap(void);

  static void max_layers(uint8_t max);

  void begin(void) final;

  static uint16_t keymap_base(void);

  static Key getKey(uint8_t layer, byte row, byte col);
  static Key getKeyOverride(uint8_t layer, byte row, byte col);

  static bool focusKeymap(const char *command);
  static bool focusKeymapLayer(const char *command);
  static bool focusKeymapTransfer(const char *command);

  static void updateKey(uint16_t base_pos, Key key);

 private:
  static uint16_t keymap_base_;
  static uint8_t max_layers_;

  static Key parseKey(void);
  static void printKey(Key key);
};
};

extern kaleidoscope::EEPROMKeymap EEPROMKeymap;

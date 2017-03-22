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

namespace KaleidoscopePlugins {
  class EEPROMKeymap : public KaleidoscopePlugin {
  public:
    EEPROMKeymap (void);

    virtual void begin (void) final;

    static void reserveSpace (uint8_t layers);
    static uint16_t base (void);

    static Key getKey (uint8_t layer, byte row, byte col);
    static Key getKeyOverride (uint8_t layer, byte row, byte col);

    static bool focusKeymap (const char *command);
    static bool focusKeymapTransfer (const char *command);

    static void updateKey (uint16_t basePos, Key key);

  private:
    static uint16_t keymapBase;
    static uint8_t maxLayers;

    static Key parseKey (void);
    static void printKey (Key key);
  };
};

extern KaleidoscopePlugins::EEPROMKeymap EEPROMKeymap;

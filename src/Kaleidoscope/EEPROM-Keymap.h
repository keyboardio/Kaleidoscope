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

#define FOCUS_HOOK_KEYMAP FOCUS_HOOK(EEPROMKeymap.focusKeymap,          \
                                     "keymap.dump\n"                    \
                                     "-----------\n"                    \
                                     "Dumps the keymap from EEPROM.\n\n" \
                                     "keymap.upload <flag keyCode>...\n" \
                                     "-------------------------------\n" \
                                     "Uploads a new keymap to EEPROM."  \
                                     "Starts at layer 0, row 0, column 0, " \
                                     "and continues as long as there is data on the line.\n\n" \
                                     "keymap.setPos layer row column flag keyCode\n" \
                                     "-------------------------------------------\n" \
                                     "Sets the key at the specified `layer`, `row`, and `column` " \
                                     "to the key with `flag`, and `keyCode` properties.\n\n" \
                                     "keymap.getPos layer row column\n" \
                                     "------------------------------\n" \
                                     "Dump the key at `layer`, `row`, and `column`.\n\n" \
                                     "keymap.transfer layer\n"          \
                                     "---------------------\n"          \
                                     "Transfers the `layer` from PROGMEM to EEPROM.")

namespace KaleidoscopePlugins {
  class EEPROMKeymap : public KaleidoscopePlugin {
  public:
    EEPROMKeymap (void);

    virtual void begin (void) final;

    static Key getKey (uint8_t layer, byte row, byte col);
    static uint16_t base (void);
    static bool focusKeymap (const char *command);

  private:
    static uint16_t keymapBase;
    static uint8_t maxLayers;

    static void updateKey (uint16_t basePos, Key key);
  };
};

extern KaleidoscopePlugins::EEPROMKeymap EEPROMKeymap;

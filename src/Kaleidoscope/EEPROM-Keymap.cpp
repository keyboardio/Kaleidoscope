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

#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-EEPROM-Keymap.h>

namespace KaleidoscopePlugins {
  uint16_t EEPROMKeymap::keymapBase;
  uint8_t EEPROMKeymap::maxLayers;

  EEPROMKeymap::EEPROMKeymap (void) {
  }

  void
  EEPROMKeymap::begin (void) {
    USE_PLUGINS (&::EEPROMSettings);

    uint16_t layerSize = ROWS * COLS * 2;
    maxLayers = (E2END - ::EEPROMSettings.endOfSettings ()) / layerSize;
    keymapBase = ::EEPROMSettings.endOfSettings () + 1;
  }

  Key
  EEPROMKeymap::getKey (uint8_t layer, byte row, byte col) {
    Key key;

    if (layer >= maxLayers)
      return Key_NoKey;

    uint16_t pos = ((layer * ROWS * COLS) + (row * COLS) + col) * 2;

    key.flags = EEPROM.read (keymapBase + pos);
    key.keyCode = EEPROM.read (keymapBase + pos + 1);

    if (key.raw == 0xffff)
      return Key_NoKey;

    return key;
  }

  uint16_t
  EEPROMKeymap::base (void) {
    return keymapBase;
  }

  void
  EEPROMKeymap::updateKey (uint16_t basePos, Key key) {
    EEPROM.update (keymapBase + basePos * 2, key.flags);
    EEPROM.update (keymapBase + basePos * 2 + 1, key.keyCode);
  }

  Key
  EEPROMKeymap::parseKey (void) {
    Key key;

    key.flags = Serial.parseInt ();
    key.keyCode = Serial.parseInt ();

    return key;
  }

  void
  EEPROMKeymap::printKey (Key k) {
    if (k.flags < 10)
      Serial.print (F(" "));
    if (k.flags < 100)
      Serial.print (F(" "));
    Serial.print (k.flags);
    Serial.print (F(" "));

    if (k.keyCode < 10)
      Serial.print (F(" "));
    if (k.keyCode < 100)
      Serial.print (F(" "));
    Serial.print (k.keyCode);
  }

  bool
  EEPROMKeymap::focusKeymap (const char *command) {
    enum {
      UPLOAD,
      DUMP,
      TRANSFER,
    } subCommand;

    if (strncmp_P (command, PSTR ("keymap."), 7) != 0)
      return false;

    if (strcmp_P (command + 7, PSTR ("upload")) == 0)
      subCommand = UPLOAD;
    else if (strcmp_P (command + 7, PSTR ("dump")) == 0)
      subCommand = DUMP;
    else if (strcmp_P (command + 7, PSTR ("transfer")) == 0)
      subCommand = TRANSFER;
    else
      return false;

    switch (subCommand) {
    case UPLOAD:
      {
        uint16_t i = 0;
        while ((Serial.peek () != '\n') && (i < ROWS * COLS * maxLayers)) {
          updateKey (i, parseKey ());
          i++;
        }
        break;
      }

    case DUMP:
      {
        for (uint8_t layer = 0; layer < 4; layer++) {
          for (uint8_t row = 0; row < ROWS; row++) {
            for (uint8_t col = 0; col < COLS; col++) {
              Key k = getKey (layer, row, col);

              printKey (k);

              if (col < COLS - 1)
                Serial.print (F(" | "));
            }
            Serial.println ();
          }
          Serial.println ();
        }
        break;
      }

    case TRANSFER:
      {
        uint8_t layer = Serial.parseInt ();

        for (uint8_t row = 0; row < ROWS; row++) {
          for (uint8_t col = 0; col < COLS; col++) {
            Key k = Layer.getKeyFromPROGMEM (layer, row, col);
            uint16_t pos = ((layer * ROWS * COLS) + (row * COLS) + col);

            updateKey (pos, k);
          }
        }

        break;
      }

    }

    Serial.read ();

    return true;
  }

};

KaleidoscopePlugins::EEPROMKeymap EEPROMKeymap;

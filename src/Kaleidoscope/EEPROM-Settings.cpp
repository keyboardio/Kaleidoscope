/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Settings -- Basic EEPROM settings plugin for Kaleidoscope.
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

namespace KaleidoscopePlugins {
  struct EEPROMSettings::settings EEPROMSettings::settings;
  bool EEPROMSettings::_isValid;

  EEPROMSettings::EEPROMSettings (void) {
  }

  void
  EEPROMSettings::begin (void) {
    _isValid = true;
    EEPROM.get (0, settings);

    if (settings.magic[0] != 'K' || settings.magic[1] != 'S') {
      settings.magic[0] = 'K';
      settings.magic[1] = 'S';
      settings.endOfSettings = EEPROM_SETTINGS_RESERVED - 1;

      return update ();
    }

    if (settings.endOfSettings != EEPROM_SETTINGS_RESERVED - 1)
      _isValid = false;
  }

  uint16_t
  EEPROMSettings::endOfSettings (void) {
    if (!isValid ())
      return 0;
    return settings.endOfSettings;
  }

  bool
  EEPROMSettings::isValid (void) {
    return _isValid;
  }

  void
  EEPROMSettings::update (void) {
    EEPROM.put (0, settings);
  }
};

KaleidoscopePlugins::EEPROMSettings EEPROMSettings;

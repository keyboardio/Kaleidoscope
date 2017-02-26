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

#pragma once

#include <Kaleidoscope.h>
#include <EEPROM.h>

#ifndef EEPROM_SETTINGS_RESERVED
#define EEPROM_SETTINGS_RESERVED ((E2END + 1) / 2)
#endif

namespace KaleidoscopePlugins {
  class EEPROMSettings : public KaleidoscopePlugin {
  public:
    EEPROMSettings (void);

    virtual void begin (void) final;

    static uint16_t endOfSettings (void);
    static void update (void);
    static bool isValid (void);
    static void invalidate (void);
    static uint8_t version (void);
    static void version (uint8_t ver);

  private:
    static bool _isValid;
    static struct settings {
      char magic[2];
      uint8_t version;
      uint16_t endOfSettings;
    } settings;
  };
};

extern KaleidoscopePlugins::EEPROMSettings EEPROMSettings;

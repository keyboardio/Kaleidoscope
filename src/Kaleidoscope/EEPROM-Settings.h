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

namespace KaleidoscopePlugins {
class EEPROMSettings : public KaleidoscopePlugin {
 public:
  EEPROMSettings(void);

  void begin(void) final;

  static void update(void);
  static bool isValid(void);
  static void invalidate(void);
  static uint8_t version(void);
  static void version(uint8_t ver);

  static uint16_t requestSlice(uint16_t size);
  static void seal(void);
  static uint16_t crc(void);
  static uint16_t used(void);

 private:
  static uint16_t nextStart;
  static bool _isValid;
  static bool sealed;

  static struct settings {
    char magic[2];
    uint8_t version;
    uint16_t crc;
  } settings;
};
};

extern KaleidoscopePlugins::EEPROMSettings EEPROMSettings;

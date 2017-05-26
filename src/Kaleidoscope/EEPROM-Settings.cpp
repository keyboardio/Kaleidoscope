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
#include "crc.h"

namespace KaleidoscopePlugins {
struct EEPROMSettings::settings EEPROMSettings::settings;
bool EEPROMSettings::_isValid;
bool EEPROMSettings::sealed;
uint16_t EEPROMSettings::nextStart = sizeof (EEPROMSettings::settings);

EEPROMSettings::EEPROMSettings (void) {
}

void
EEPROMSettings::begin (void) {
    EEPROM.get (0, settings);
}

bool
EEPROMSettings::isValid (void) {
    return _isValid;
}

uint16_t
EEPROMSettings::crc (void) {
    if (sealed)
        return settings.crc;
    return 0;
}

void
EEPROMSettings::seal (void) {
    sealed = true;

    CRC.finalize ();

    if (settings.magic[0] != 'K' || settings.magic[1] != 'S') {
        settings.magic[0] = 'K';
        settings.magic[1] = 'S';
        settings.version = 0;
        settings.crc = CRC.crc;

        return update ();
    }

    if (settings.crc != CRC.crc)
        _isValid = false;
}

uint16_t
EEPROMSettings::requestSlice (uint16_t size) {
    if (sealed)
        return 0;

    uint16_t start = nextStart;
    nextStart += size;

    CRC.update ((const void *)&size, sizeof (size));

    return start;
}

void
EEPROMSettings::invalidate (void) {
    _isValid = false;
}

uint16_t
EEPROMSettings::used (void) {
    return nextStart;
}

void
EEPROMSettings::update (void) {
    settings.crc = CRC.crc;

    EEPROM.put (0, settings);
    _isValid = true;
}

uint8_t
EEPROMSettings::version (void) {
    return settings.version;
}

void
EEPROMSettings::version (uint8_t ver) {
    settings.version = ver;
    update ();
}
};

KaleidoscopePlugins::EEPROMSettings EEPROMSettings;

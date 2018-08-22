/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Settings -- Basic EEPROM settings plugin for Kaleidoscope.
 * Copyright (C) 2017  Keyboard.io, Inc
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

namespace kaleidoscope {
namespace eeprom_settings {

bool settingsFocusHook(const char *command);
bool eepromFocusHook(const char *command);

}
}

#define FOCUS_HOOK_SETTINGS FOCUS_HOOK                                  \
  (kaleidoscope::eeprom_settings::settingsFocusHook,                    \
   "settings.valid?\n"                                                  \
   "settings.version\n"                                                 \
   "settings.crc")

#define FOCUS_HOOK_EEPROM FOCUS_HOOK                                    \
  (kaleidoscope::eeprom_settings::eepromFocusHook,                      \
   "eeprom.free\n"                                                      \
   "eeprom.contents")

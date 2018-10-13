/* -*- mode: c++ -*-
 * Kaleidoscope-EEPROM-Settings -- Basic EEPROM settings plugin for Kaleidoscope.
 * Copyright (C) 2017, 2018  Keyboard.io, Inc
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
namespace plugin {
namespace eeprom {
class FocusSettingsCommand : public kaleidoscope::Plugin {
 public:
  FocusSettingsCommand() {}

  EventHandlerResult onFocusEvent(const char *command);
};

class FocusEEPROMCommand : public kaleidoscope::Plugin {
 public:
  FocusEEPROMCommand() {}

  EventHandlerResult onFocusEvent(const char *command);
};

}
}
}

extern kaleidoscope::plugin::eeprom::FocusSettingsCommand FocusSettingsCommand;
extern kaleidoscope::plugin::eeprom::FocusEEPROMCommand FocusEEPROMCommand;

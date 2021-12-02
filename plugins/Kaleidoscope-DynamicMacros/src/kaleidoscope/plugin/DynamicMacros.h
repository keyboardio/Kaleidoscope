/* DynamicMacros - Dynamic macro support for Kaleidoscope.
 * Copyright (C) 2019, 2021  Keyboard.io, Inc.
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

#include "kaleidoscope/Runtime.h"
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-Macros.h>
#include <Kaleidoscope-Ranges.h>

#define DM(n) Key(kaleidoscope::ranges::DYNAMIC_MACRO_FIRST + n)
#define MACRO_SOURCE_EEPROM 1

namespace kaleidoscope {
namespace plugin {

class DynamicMacros : public kaleidoscope::Plugin {
 public:
  EventHandlerResult onNameQuery();
  EventHandlerResult onKeyEvent(KeyEvent &event);
  EventHandlerResult onFocusEvent(const char *command);

  static void reserve_storage(uint16_t size);
  static void setup(uint16_t storage_size);

  void play(uint8_t seq_id);

  static uint8_t readMacroByteFromEEPROM(const macro_t *ptr, uint8_t source);
  static uint8_t readMacroByte(const macro_t *ptr, uint8_t source);

 private:
  static uint16_t storage_base_;
  static uint16_t storage_size_;
  static uint16_t map_[31];
  static void updateDynamicMacroCache();
};

} // namespace plugin
} // namespace kaleidoscope

extern kaleidoscope::plugin::DynamicMacros DynamicMacros;

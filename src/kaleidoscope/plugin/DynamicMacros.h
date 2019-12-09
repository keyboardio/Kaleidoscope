/* DynamicMacros - Dynamic macro support for Kaleidoscope.
 * Copyright (C) 2019  Keyboard.io, Inc.
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

#include <stdint.h>                             // for uint16_t, uint8_t
#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin
#include "Kaleidoscope-Ranges.h"

namespace kaleidoscope {
class Key;
}

#define DM(n) Key(kaleidoscope::ranges::DYNAMIC_MACRO_FIRST + n)

namespace kaleidoscope {
namespace plugin {

class DynamicMacros : public kaleidoscope::Plugin {
 public:
  DynamicMacros(void) {}

  EventHandlerResult onKeyswitchEvent(Key &mappedKey, KeyAddr key_addr, uint8_t keyState);
  EventHandlerResult onFocusEvent(const char *command);

  static void reserve_storage(uint16_t size);

  void play(uint8_t seq_id);

 private:
  static uint16_t storage_base_;
  static uint16_t storage_size_;
  static uint16_t map_[31];
  static void updateDynamicMacroCache(void);
};

}
}

extern kaleidoscope::plugin::DynamicMacros DynamicMacros;

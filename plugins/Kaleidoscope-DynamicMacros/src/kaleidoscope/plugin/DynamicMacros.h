/* DynamicMacros - Dynamic macro support for Kaleidoscope.
 * Copyright (C) 2019-2022  Keyboard.io, Inc.
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

#include <Kaleidoscope-MacroSupport.h>  // for MacroSupport
#include <Kaleidoscope-Ranges.h>        // for DYNAMIC_MACRO_FIRST
#include <stdint.h>                     // for uint16_t, uint8_t

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/plugin.h"                // for Plugin

#define DM(n) ::kaleidoscope::plugin::DynamicMacrosKey(n)

namespace kaleidoscope {
namespace plugin {

constexpr Key DynamicMacrosKey(uint8_t n) {
  return Key(kaleidoscope::ranges::DYNAMIC_MACRO_FIRST + n);
}

class DynamicMacros : public kaleidoscope::Plugin {
 public:
  EventHandlerResult onNameQuery();
  EventHandlerResult onKeyEvent(KeyEvent &event);
  EventHandlerResult onFocusEvent(const char *input);
  EventHandlerResult beforeReportingState(const KeyEvent &event) {
    return ::MacroSupport.beforeReportingState(event);
  }

  void reserve_storage(uint16_t size);

  void play(uint8_t seq_id);

 private:
  uint16_t storage_base_;
  uint16_t storage_size_;
  uint16_t map_[32];
  uint8_t macro_count_;
  uint8_t updateDynamicMacroCache();

  inline void press(Key key) { ::MacroSupport.press(key); }
  inline void release(Key key) { ::MacroSupport.release(key); }
  inline void tap(Key key) const { ::MacroSupport.tap(key); }
  inline void clear() { ::MacroSupport.clear(); }
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::DynamicMacros DynamicMacros;

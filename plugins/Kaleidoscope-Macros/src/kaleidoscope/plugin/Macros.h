/* Kaleidoscope-Macros - Macro keys for Kaleidoscope.
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
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

#include "kaleidoscope/plugin/Macros/MacroKeyDefs.h"
#include "kaleidoscope/plugin/Macros/MacroSteps.h"
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/key_events.h"

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState);

#if !defined(MAX_CONCURRENT_MACROS)
#define MAX_CONCURRENT_MACROS 8
#endif

struct MacroKeyEvent {
  byte key_code;
  byte key_id;
  byte key_state;
};

namespace kaleidoscope {
namespace plugin {

class Macros_ : public kaleidoscope::Plugin {
 public:
  Macros_(void) {}

  static MacroKeyEvent active_macros[MAX_CONCURRENT_MACROS];
  static byte active_macro_count;
  static void addActiveMacroKey(byte key_code, byte key_id, byte key_state) {
    // If we've got too many active macros, give up:
    if (active_macro_count >= MAX_CONCURRENT_MACROS) {
      return;
    }
    active_macros[active_macro_count].key_code = key_code;
    active_macros[active_macro_count].key_id = key_id;
    active_macros[active_macro_count].key_state = key_state;
    ++active_macro_count;
  }

  EventHandlerResult onKeyswitchEvent(Key &mappedKey, KeyAddr key_addr, uint8_t keyState);
  EventHandlerResult beforeReportingState();
  EventHandlerResult afterEachCycle();

  void play(const macro_t *macro_p);

  /* What follows below, is a bit of template magic that allows us to use
     Macros.type() with any number of arguments, without having to use a
     sentinel. See the comments on Runtime.use() for more details - this is
     the same trick.
   */
  inline const macro_t *type() {
    return MACRO_NONE;
  }
  const macro_t *type(const char *string);
  template <typename... Strings>
  const macro_t *type(const char *first, Strings&&... strings) {
    type(first);
    return type(strings...);
  }

  static KeyAddr key_addr;

 private:
  Key lookupAsciiCode(uint8_t ascii_code);
  bool isMacroKey(Key key);
};

}
}

extern kaleidoscope::plugin::Macros_ Macros;

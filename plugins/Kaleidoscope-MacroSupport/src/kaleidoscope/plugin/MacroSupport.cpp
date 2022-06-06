/* Kaleidoscope-MacroSupport - Macros support functions for Kaleidoscope
 * Copyright (C) 2022  Keyboard.io, Inc.
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

#include "kaleidoscope/plugin/MacroSupport.h"

#include <Arduino.h>                   // for F, __FlashStringHelper
#include <Kaleidoscope-FocusSerial.h>  // for Focus, FocusSerial
#include <stdint.h>                    // for uint8_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/key_defs.h"              // for Key, Key_NoKey
#include "kaleidoscope/keyswitch_state.h"       // for INJECTED, IS_PRESSED, WAS_PRESSED

// =============================================================================
// `Macros` plugin code
namespace kaleidoscope {
namespace plugin {

constexpr uint8_t press_state   = IS_PRESSED | INJECTED;
constexpr uint8_t release_state = WAS_PRESSED | INJECTED;

// -----------------------------------------------------------------------------
// Public helper functions

void MacroSupport::press(Key key) {
  Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), press_state, key});
  // This key may remain active for several subsequent events, so we need to
  // store it in the active macro keys array.
  for (Key &macro_key : active_macro_keys_) {
    if (macro_key == Key_NoKey) {
      macro_key = key;
      break;
    }
  }
}

void MacroSupport::release(Key key) {
  // Before sending the release event, we need to remove the key from the active
  // macro keys array, or it will get inserted into the report anyway.
  for (Key &macro_key : active_macro_keys_) {
    if (macro_key == key) {
      macro_key = Key_NoKey;
    }
  }
  Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), release_state, key});
}

void MacroSupport::clear() {
  // Clear the active macro keys array.
  for (Key &macro_key : active_macro_keys_) {
    if (macro_key == Key_NoKey)
      continue;
    Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), release_state, macro_key});
    macro_key = Key_NoKey;
  }
}

void MacroSupport::tap(Key key) const {
  // No need to call `press()` & `release()`, because we're immediately
  // releasing the key after pressing it. It is possible for some other plugin
  // to insert an event in between, but very unlikely.
  Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), press_state, key});
  Runtime.handleKeyEvent(KeyEvent{KeyAddr::none(), release_state, key});
}

// -----------------------------------------------------------------------------
// Event handlers

EventHandlerResult MacroSupport::beforeReportingState(const KeyEvent &event) {
  // Do this in beforeReportingState(), instead of `onAddToReport()` because
  // `live_keys` won't get updated until after the macro sequence is played from
  // the keypress. This could be changed by either updating `live_keys` manually
  // ahead of time, or by executing the macro sequence on key release instead of
  // key press. This is probably the simplest solution.
  for (Key key : active_macro_keys_) {
    if (key != Key_NoKey)
      Runtime.addToReport(key);
  }
  return EventHandlerResult::OK;
}

EventHandlerResult MacroSupport::onNameQuery() {
  return ::Focus.sendName(F("MacroSupport"));
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::MacroSupport MacroSupport;

/* Kaleidoscope-Macros - Macro keys for Kaleidoscope.
 * Copyright (C) 2017-2022  Keyboard.io, Inc.
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
#include <Kaleidoscope-Ranges.h>        // for MACRO_FIRST, MACRO_LAST
#include <stdint.h>                     // for uint8_t

#include "kaleidoscope/KeyEvent.h"                  // for KeyEvent
#include "kaleidoscope/event_handler_result.h"      // for EventHandlerResult
#include "kaleidoscope/key_defs.h"                  // for Key
#include "kaleidoscope/plugin.h"                    // for Plugin
#include "kaleidoscope/plugin/Macros/MacroSteps.h"  // for macro_t, MACRO_NONE

// =============================================================================
// Define this function in a Kaleidoscope sketch in order to trigger Macros.
const macro_t *macroAction(uint8_t macro_id, KeyEvent &event);

namespace kaleidoscope {
namespace plugin {

class Macros : public kaleidoscope::Plugin {
 public:
  /// Send a key press event from a Macro
  ///
  /// Generates a new `KeyEvent` and calls `Runtime.handleKeyEvent()` with the
  /// specified `key`, then stores that `key` in an array of active macro key
  /// values. This allows the macro to press one key and keep it active when a
  /// subsequent key event is sent as part of the same macro sequence.
  inline void press(Key key) {
    ::MacroSupport.press(key);
  }

  /// Send a key release event from a Macro
  ///
  /// Generates a new `KeyEvent` and calls `Runtime.handleKeyEvent()` with the
  /// specified `key`, then removes that key from the array of active macro
  /// keys (see `Macros.press()`).
  inline void release(Key key) {
    ::MacroSupport.release(key);
  }

  /// Clear all virtual keys held by Macros
  ///
  /// This function clears the active macro keys array, sending a release event
  /// for each key stored there.
  inline void clear() {
    ::MacroSupport.clear();
  }

  /// Send a key "tap event" from a Macro
  ///
  /// Generates two new `KeyEvent` objects, one each to press and release the
  /// specified `key`, passing both in sequence to `Runtime.handleKeyEvent()`.
  inline void tap(Key key) const {
    ::MacroSupport.tap(key);
  }

  /// Play a macro sequence of key events
  void play(const macro_t *macro_ptr);

  // Templates provide a `type()` function that takes a variable number of
  // `char*` (string) arguments, in the form of a list of strings stored in
  // PROGMEM, of the form `Macros.type(PSTR("Hello "), PSTR("world!"))`.
  inline const macro_t *type() const {
    return MACRO_NONE;
  }
  const macro_t *type(const char *string) const;
  template<typename... Strings>
  const macro_t *type(const char *first, Strings &&...strings) const {
    type(first);
    return type(strings...);
  }

  // ---------------------------------------------------------------------------
  // Event handlers
  EventHandlerResult onNameQuery();
  EventHandlerResult onKeyEvent(KeyEvent &event);
  EventHandlerResult beforeReportingState(const KeyEvent &event) {
    return ::MacroSupport.beforeReportingState(event);
  }

 private:
  // Translate and ASCII character value to a corresponding `Key`
  Key lookupAsciiCode(uint8_t ascii_code) const;

  // Test for a key that encodes a macro ID
  bool isMacrosKey(Key key) const {
    if (key >= ranges::MACRO_FIRST && key <= ranges::MACRO_LAST)
      return true;
    return false;
  }
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::Macros Macros;

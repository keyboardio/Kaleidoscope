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

#pragma once

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/plugin.h"                // for Plugin

// =============================================================================
// The number of simultaneously-active `Key` values that a macro can have
// running during a call to `Macros.play()`. I don't know if it's actually
// possible to override this by defining it in a sketch before including
// "Kaleidoscope-Macros.h", but probably not.
#if !defined(MAX_CONCURRENT_MACRO_KEYS)
#define MAX_CONCURRENT_MACRO_KEYS 8
#endif

namespace kaleidoscope {
namespace plugin {

class MacroSupport : public Plugin {
 public:
  /// Send a key press event from a Macro
  ///
  /// Generates a new `KeyEvent` and calls `Runtime.handleKeyEvent()` with the
  /// specified `key`, then stores that `key` in an array of active macro key
  /// values. This allows the macro to press one key and keep it active when a
  /// subsequent key event is sent as part of the same macro sequence.
  void press(Key key);

  /// Send a key release event from a Macro
  ///
  /// Generates a new `KeyEvent` and calls `Runtime.handleKeyEvent()` with the
  /// specified `key`, then removes that key from the array of active macro
  /// keys (see `Macros.press()`).
  void release(Key key);

  /// Clear all virtual keys held by Macros
  ///
  /// This function clears the active macro keys array, sending a release event
  /// for each key stored there.
  void clear();

  /// Send a key "tap event" from a Macro
  ///
  /// Generates two new `KeyEvent` objects, one each to press and release the
  /// specified `key`, passing both in sequence to `Runtime.handleKeyEvent()`.
  void tap(Key key) const;

  // ---------------------------------------------------------------------------
  // Event handlers
  EventHandlerResult onNameQuery();
  EventHandlerResult beforeReportingState(const KeyEvent &event);

 private:
  // An array of key values that are active while a macro sequence is playing
  Key active_macro_keys_[MAX_CONCURRENT_MACRO_KEYS];
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::MacroSupport MacroSupport;

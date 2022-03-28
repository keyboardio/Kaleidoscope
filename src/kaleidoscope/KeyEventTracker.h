/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2021  Keyboard.io, Inc.
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

#include "kaleidoscope/KeyEvent.h"  // for KeyEvent, KeyEventId

namespace kaleidoscope {

/// An event tracker for plugins that implement `onKeyswitchEvent()`
///
/// Plugins that implement the `onKeyswitchEvent()` are required to agree to a
/// contract wherein they promise to only process each event once.  They may
/// delay an event by returning `EventHandlerResult::ABORT` when they first
/// encounter an event, then re-start it (in response to a subsequent event or a
/// timeout) by calling `Runtime.handleKeyswitchEvent()`.  When they do so, they
/// must take pains to ensure that they ignore that event when it gets passed to
/// their `onKeyswitchEvent()` handler a second time.  In addition, a subsequent
/// plugin's `onKeyswitchEvent()` handler might do the same thing, and the event
/// must be ignored in that case as well. This `KeyEventTracker` class is a
/// helper that makes it easy for plugins to abide by the terms of the
/// `onKeyswitchEvent()` contract.
///
/// All that's required is adding a private member variable to the plugin's
/// class definition, as follows:
///
/// ```c++
/// class MyPlugin : public Plugin {
///  private:
///   KeyEventTracker event_tracker_;
///  public:
///   EventHandlerResult onKeyswitchEvent(KeyEvent &event);
/// };
/// ```
///
/// Then, in the definition of that plugin's `onKeyswitchEvent()` function, use
/// the tracker's `shouldIgnore()` function to let already-processed events pass
/// through:
///
/// ```c++
/// EventHandlerResult MyPlugin::onKeyswitchEvent(KeyEvent &event) {
///   if (event_tracker_.shouldIgnore(event))
///     return EventHandlerResult::OK;
///   ...
/// }
/// ```
///
/// As a side effect, the `shouldIgnore()` function will record the `id` of the
/// event, if it is not one that the tracker has seen "recently".  It works by
/// keeping track of the "newest" event ID that it has seen, by subtracting the
/// ID of the event passed to it from the current "newest" event ID.  If the
/// result is greater than zero, the event is considered to be "new" and the
/// tracked ID is updated.  The mechanism isn't perfect; if 128 (`KeyEventId`
/// stores ony byte of data) new event IDs are generated without any of them
/// reaching the plugin's `onKeyswitchEvent()` handler, it is theoretically
/// possible to overflow and a new event will be treated as old.

class KeyEventTracker {

 private:
  // The ID of the "newest" event this tracker has seen.
  KeyEventId last_id_seen_{-1};

 public:
  /// Check if an event should be ignored by the client plugin
  ///
  /// This function should be called by a plugin's `onKeyswitchEvent()` handler
  /// function to determined if the `event` has an `id` member that this tracker
  /// has seen "recently".
  bool shouldIgnore(const KeyEvent &event) {
    return !isNew(event);
  }

  /// Report if a given event is "new" to this tracker
  ///
  /// Returns `true` if the input event is newer than the latest event the
  /// tracker has seen.  If it is newer, also update the tracker to reflect the
  /// new event ID.
  bool isNew(const KeyEvent &event) {
    KeyEventId offset = event.id() - last_id_seen_;
    if (offset > 0) {
      last_id_seen_ = event.id();
      return true;
    }
    return false;
  }
};

}  // namespace kaleidoscope

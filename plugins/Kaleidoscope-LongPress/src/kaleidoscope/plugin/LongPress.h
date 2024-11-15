/* -*- mode: c++ -*-
 * Kaleidoscope-LongPress -- Provide different key strokes on long press
 * Copyright (C) 2024  Keyboard.io, Inc
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

#include <stdint.h>  // for uint8_t, uint16_t

#include "kaleidoscope/KeyAddrEventQueue.h"     // for KeyAddrEventQueue
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/KeyEventTracker.h"       // for KeyEventTracker
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/plugin.h"                // for Plugin

#include "kaleidoscope/plugin/LongPressAutoShift.h"  // for longpress::AutoShiftCategories
#include "kaleidoscope/plugin/LongPressConstants.h"  // for ALL_LAYERS

namespace kaleidoscope {
namespace plugin {

// A mapping for a long-press configuration. Either a KeyAddr or a Key (but
// not both) must be given as the source of the event and a Key as the
// result of a long-press. A specific layer may be specified to restrict
// the long-press behaviour to that layer.
struct LongPressKey {
  // The layer to react on to this long press
  uint8_t layer;
  // The physical key that should result in a different value on long press.
  KeyAddr addr;
  // The logical key that should result in a different value on long press.
  Key key;
  // The alternate logical Key value that should be produced on long press.
  Key longpress_result;

  // This is the constructor that should be used when creating a
  // LongPressKey object for a physical KeyAddr in the PROGMEM array
  // that will be used by explicit mappings (i.e. in the `LONGPRESS()`
  // macro).
  constexpr LongPressKey(uint8_t layer, KeyAddr addr, Key longpress_result)
    : layer(layer), addr(addr), key(Key_Transparent), longpress_result(longpress_result) {}

  // This is the constructor that should be used when creating a
  // LongPressKey object for a logical Key in the PROGMEM array
  // that will be used by explicit mappings (i.e. in the `LONGPRESS()`
  // macro).
  constexpr LongPressKey(uint8_t layer, Key key, Key longpress_result)
    : layer(layer), addr(KeyAddr::none()), key(key), longpress_result(longpress_result) {}

  // This constructor is here so that we can create an empty LongPressKey object in RAM
  // into which we can copy the values from a PROGMEM LongPressKey object.
  LongPressKey() = default;
};

// =============================================================================
/// Kaleidoscope plugin for long-press keys
///
/// This plugin allows the user to "long-press" keys to produce different
/// key strokes than on normal key presses.
/// Shortcut methods are provided to generate shifted letters on long-press
/// without having to configure each letter explicitly.
class LongPress : public Plugin {

 public:
  // ---------------------------------------------------------------------------
  // This lets the LongPressConfig plugin access the internal config variables
  // directly. Mainly useful for calls to `Runtime.storage.get()`.
  friend class LongPressConfig;

  // ---------------------------------------------------------------------------
  // Configuration functions

  /// Returns `true` if LongPress is active, `false` otherwise
  bool enabled() {
    return settings_.enabled;
  }
  /// Activates the LongPress plugin (held keys will produce different Keys)
  void enable() {
    settings_.enabled = true;
  }
  /// Deactivates the LongPress plugin (held keys will not produce different Keys)
  void disable();
  /// Turns LongPress on if it's off, and vice versa
  void toggle() {
    if (settings_.enabled) {
      disable();
    } else {
      enable();
    }
  }

  /// Returns the hold time required to trigger long press (in ms)
  uint16_t timeout() {
    return settings_.timeout;
  }
  /// Sets the hold time required to trigger long press (in ms)
  void setTimeout(uint16_t new_timeout) {
    settings_.timeout = new_timeout;
  }

  /// Returns the set of categories currently eligible for auto-shift
  longpress::AutoShiftCategories enabledAutoShiftCategories() {
    return settings_.enabled_categories;
  }

  /// Adds `category` to the set eligible for auto-shift
  ///
  /// Possible values for `category` are:
  ///  - `longpress::AutoShiftCategories::noKeys()`
  ///  - `longpress::AutoShiftCategories::letterKeys()`
  ///  - `longpress::AutoShiftCategories::numberKeys()`
  ///  - `longpress::AutoShiftCategories::symbolKeys()`
  ///  - `longpress::AutoShiftCategories::arrowKeys()`
  ///  - `longpress::AutoShiftCategories::functionKeys()`
  ///  - `longpress::AutoShiftCategories::printableKeys()`
  ///  - `longpress::AutoShiftCategories::allKeys()`
  void enableAutoshift(longpress::AutoShiftCategories category) {
    settings_.enabled_categories.add(category);
  }
  /// Removes a `Key` category from the set eligible for auto-shift
  void disableAutoshift(longpress::AutoShiftCategories category) {
    settings_.enabled_categories.remove(category);
  }
  /// Replaces the list of `Key` categories eligible for auto-shift
  void setAutoshiftEnabled(longpress::AutoShiftCategories categories) {
    settings_.enabled_categories = categories;
  }
  /// Returns `true` if the given category is eligible for auto-shift
  bool isAutoshiftEnabled(longpress::AutoShiftCategories category) {
    return settings_.enabled_categories.contains(category);
  }

  /// The category representing no keys
  static constexpr longpress::AutoShiftCategories noKeys() {
    return longpress::AutoShiftCategories::noKeys();
  }
  /// The category representing letter keys
  static constexpr longpress::AutoShiftCategories letterKeys() {
    return longpress::AutoShiftCategories::letterKeys();
  }
  /// The category representing number keys (on the number row)
  static constexpr longpress::AutoShiftCategories numberKeys() {
    return longpress::AutoShiftCategories::numberKeys();
  }
  /// The category representing other printable symbol keys
  static constexpr longpress::AutoShiftCategories symbolKeys() {
    return longpress::AutoShiftCategories::symbolKeys();
  }
  /// The category representing arrow keys
  static constexpr longpress::AutoShiftCategories arrowKeys() {
    return longpress::AutoShiftCategories::arrowKeys();
  }
  /// The category representing function keys
  static constexpr longpress::AutoShiftCategories functionKeys() {
    return longpress::AutoShiftCategories::functionKeys();
  }
  /// Letters, numbers, and other symbols
  static constexpr longpress::AutoShiftCategories printableKeys() {
    return longpress::AutoShiftCategories::printableKeys();
  }
  /// All non-modifier keyboard keys
  static constexpr longpress::AutoShiftCategories allKeys() {
    return longpress::AutoShiftCategories::allKeys();
  }

  // ---------------------------------------------------------------------------
  /// Determines which keys will trigger auto-shift if held long enough
  ///
  /// This function can be overridden by the user sketch to configure which keys
  /// can trigger auto-shift.
  bool isAutoShiftable(Key key);

  // ---------------------------------------------------------------------------
  // Event handlers
  EventHandlerResult onKeyswitchEvent(KeyEvent &event);
  EventHandlerResult afterEachCycle();

  template<uint8_t _explicitmappings_count>
  void configureLongPresses(LongPressKey const (&explicitmappings)[_explicitmappings_count]) {
    explicitmappings_       = explicitmappings;
    explicitmappings_count_ = _explicitmappings_count;
  }

 private:
  // ---------------------------------------------------------------------------
  /// A container for LongPress configuration settings
  struct Settings {
    /// The overall state of the plugin (on/off)
    bool enabled = true;
    /// The length of time (ms) a key must be held to trigger a long press
    uint16_t timeout = 175;
    /// The set of `Key` categories eligible to be auto-shifted
    longpress::AutoShiftCategories enabled_categories = longpress::AutoShiftCategories::noKeys();
  };
  Settings settings_;

  // ---------------------------------------------------------------------------
  // Key event queue state variables

  // A device for processing only new events
  KeyEventTracker event_tracker_;

  // The maximum number of events in the queue at a time.
  static constexpr uint8_t queue_capacity_{4};

  // The event queue stores a series of press and release events.
  KeyAddrEventQueue<queue_capacity_> queue_;

  // If there's a delayed keypress from LongPress, this stored event will
  // contain a valid `KeyAddr`.  The default constructor produces an event addr
  // of `KeyAddr::none()`, so the plugin will start in an inactive state.
  KeyEvent pending_event_;

  void flushQueue();
  void flushEvent(bool is_long_press = false);
  bool checkForRelease() const;

  /// The default function for `isAutoShiftable()`
  bool autoShiftEnabledForKey(Key key);

  /// Checks whether an explicit long-press mapping exists for either the
  /// given `addr` or `key`.
  bool isExplicitlyMapped(KeyAddr addr, Key key);

  // An array of LongPressKey objects in PROGMEM.
  LongPressKey const *explicitmappings_{nullptr};
  uint8_t explicitmappings_count_{0};

  // A cache of the current explicit config key values, so we
  // don't have to keep looking them up from PROGMEM.
  struct {
    KeyAddr addr{KeyAddr::none()};
    Key key{Key_Transparent};
    Key longpress_result{Key_Transparent};
  } mapped_key_;
};

// =============================================================================
/// Configuration plugin for persistent storage of settings
class LongPressConfig : public Plugin {
 public:
  EventHandlerResult onSetup();
  EventHandlerResult onFocusEvent(const char *input);
  void disableLongPressIfUnconfigured();

 private:
  // The base address in persistent storage for configuration data
  uint16_t settings_base_;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::LongPress LongPress;
extern kaleidoscope::plugin::LongPressConfig LongPressConfig;

#define LONGPRESS(longpress_defs...)                                              \
  {                                                                               \
    static kaleidoscope::plugin::LongPressKey const longpress_defs_[] PROGMEM = { \
      longpress_defs};                                                            \
    LongPress.configureLongPresses(longpress_defs_);                              \
  }

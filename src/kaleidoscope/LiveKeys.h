/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2021  Keyboard.io, Inc.
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

#include "kaleidoscope/KeyAddr.h"     // for KeyAddr
#include "kaleidoscope/KeyAddrMap.h"  // for KeyAddrMap<>::Iterator, KeyAddrMap
#include "kaleidoscope/KeyMap.h"      // for KeyMap
#include "kaleidoscope/key_defs.h"    // for Key, Key_Masked, Key_Inactive

namespace kaleidoscope {

/// A representation of the "live" state of the keys on the keyboard
///
/// This is structure of `Key` values, indexed by `KeyAddr` values, with one
/// entry per key on the keyboard. These entries are meant to represent the
/// current state of the keys, with two special values:
///
/// - `Key_Inactive` indicates that the given key is not active. Usually this
///   will correspond to the keyswitch being off (not pressed).
///
/// - `Key_Masked` indicates that the given key has been masked. When a key
///   release event is processed for that key, it will be reset to
///   `Key_Inactive`.
///
/// Any other value indicates that the key is active (i.e. pressed, though
/// plugins can set entries to active even when the physical keyswitches are not
/// engaged), and the `Key` value is what the that key is "sending" at the
/// time. At the end of its processing of a `KeyEvent`, Kaleidoscope will use
/// the contents of this array to populate the Keyboard HID reports.

class LiveKeys {
 public:
  // For array-style subscript addressing of entries in a read-only context:
  const Key &operator[](KeyAddr key_addr) const {
    if (key_addr.isValid()) {
      return key_map_[key_addr];
    }
    dummy_ = Key_Masked;
    return dummy_;
  }

  // For array-style subscript addressing of entries by reference. The client
  // code can alter values in the array this way.
  Key &operator[](KeyAddr key_addr) {
    if (key_addr.isValid()) {
      return key_map_[key_addr];
    }
    dummy_ = Key_Masked;
    return dummy_;
  }

  /// Set an entry to "active" with a specified `Key` value.
  void activate(KeyAddr key_addr, Key key) {
    if (key_addr.isValid())
      key_map_[key_addr] = key;
  }

  /// Deactivate an entry by setting its value to `Key_Inactive`.
  void clear(KeyAddr key_addr) {
    if (key_addr.isValid())
      key_map_[key_addr] = Key_Inactive;
  }

  /// Mask a key by setting its entry to `Key_Masked`. The key will become
  /// unmasked by Kaleidoscope on release (but not on a key press event).
  void mask(KeyAddr key_addr) {
    if (key_addr.isValid())
      key_map_[key_addr] = Key_Masked;
  }

  /// Clear the entire array by setting all values to `Key_Inactive`.
  void clear() {
    for (Key &key : key_map_) {
      key = Key_Inactive;
    }
  }

  /// Returns an iterator for use in range-based for loops:
  ///
  ///   for (Key key : live_keys.all()) {...}
  KeyMap &all() {
    return key_map_;
  }

 private:
  KeyMap key_map_;
  mutable Key dummy_{0, 0};
};

extern LiveKeys live_keys;

}  // namespace kaleidoscope

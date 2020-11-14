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

#include "kaleidoscope_internal/device.h"
#include "kaleidoscope/KeyAddr.h"
#include "kaleidoscope/key_defs.h"

namespace kaleidoscope {

// A `KeyMap` is a collection of `Key` objects, indexed by `KeyAddr`, with one
// entry per key on the keyboard.

class KeyMap {

 private:
  static constexpr uint8_t kSize_ = kaleidoscope_internal::device.numKeys();

  Key keys_[kSize_];

 public:
  // Return the number of `Key` entries in the array
  static constexpr uint8_t size() {
    return kSize_;
  }

  // To set the value of an entry:
  // key_array[key_addr] = Key_X;
  Key& operator[](KeyAddr key_addr) {
    return keys_[key_addr.toInt()];
  }

  // To get the value of an entry:
  // Key key = key_array[key_addr];
  const Key& operator[](KeyAddr key_addr) const {
    return keys_[key_addr.toInt()];
  }

  // This function sets all entries to the value `Key_Transparent`
  void clear() {
    memset(keys_, -1, sizeof(keys_));
  }

  // ---------------------------------------------------------------------------
  // The following code defines an iterator class for the `KeyMap`, such that
  // we can write the following code to get each `Key` entry in the array:
  //
  // for (Key key : key_map) {...}
 private:
  class Iterator;
  friend class KeyMap::Iterator;

 public:
  Iterator begin() {
    return {*this, KeyAddr(uint8_t(0))};
  }
  Iterator end() {
    return {*this, KeyAddr(kSize_)};
  }

 private:
  class Iterator {
   public:
    Iterator(KeyMap &array, KeyAddr key_addr)
      : array_(array), key_addr_(key_addr) {}
    bool operator!=(const Iterator &other) const {
      return key_addr_ != other.key_addr_;
    }
    Key operator*() const {
      return array_[key_addr_];
    }
    Iterator& operator++() {
      ++key_addr_;
      return *this;
    }
   private:
    KeyMap &array_;
    KeyAddr key_addr_;
  };
};

} // namespace kaleidoscope

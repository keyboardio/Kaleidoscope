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

#include <stdint.h>  // for uint8_t

#include "kaleidoscope/KeyAddr.h"  // for KeyAddr

namespace kaleidoscope {

// A `KeyAddrMap` is a collection of objects, indexed by `KeyAddr`, with one
// entry per key on the keyboard.

template<typename _ContentType, uint8_t _size>
class KeyAddrMap {

 private:
  _ContentType values_[_size];  // NOLINT(runtime/arrays)

 public:
  typedef KeyAddrMap<_ContentType, _size> ThisType;

  // Return the number of `Key` entries in the array
  static constexpr uint8_t size() {
    return _size;
  }

  // To set the value of an entry:
  // key_array[key_addr] = Key_X;
  _ContentType &operator[](KeyAddr key_addr) {
    return values_[key_addr.toInt()];
  }

  // To get the value of an entry:
  // Key key = key_array[key_addr];
  const _ContentType &operator[](KeyAddr key_addr) const {
    return values_[key_addr.toInt()];
  }

  // ---------------------------------------------------------------------------
  // The following code defines an iterator class for a class `KeyMap`, such
  // that we can write the following code to get each entry in the array:
  //
  // typedef KeyAddrMap<Key> KeyMap;
  //
  // for (Key key : key_map) {...}
  //
  // Or, if we need write access to the entries in the array:
  //
  // for (Key &key : key_map) {...}
 private:
  class Iterator;

  friend class ThisType::Iterator;

 public:
  Iterator begin() {
    return {*this, KeyAddr(uint8_t(0))};
  }
  Iterator end() {
    return {*this, KeyAddr(_size)};
  }

 private:
  class Iterator {
   public:
    Iterator(ThisType &map, KeyAddr key_addr)
      : map_(map), key_addr_(key_addr) {}
    bool operator!=(const Iterator &other) const {
      return key_addr_ != other.key_addr_;
    }
    _ContentType &operator*() const {
      return map_[key_addr_];
    }
    Iterator &operator++() {
      ++key_addr_;
      return *this;
    }

   private:
    ThisType &map_;
    KeyAddr key_addr_;
  };
};

}  // namespace kaleidoscope

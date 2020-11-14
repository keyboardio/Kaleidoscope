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

#include "kaleidoscope/KeyAddr.h"
#include "kaleidoscope/KeyMap.h"
#include "kaleidoscope/key_defs.h"

namespace kaleidoscope {

class LiveKeys {
 public:
  const Key& operator[](KeyAddr key_addr) const {
    return key_map_[key_addr];
  }
  Key& operator[](KeyAddr key_addr) {
    return key_map_[key_addr];
  }
  void activate(KeyAddr key_addr, Key key) {
    key_map_[key_addr] = key;
  }
  void clear(KeyAddr key_addr) {
    key_map_[key_addr] = Key_Transparent;
  }
  void mask(KeyAddr key_addr) {
    key_map_[key_addr] = Key_NoKey;
  }
  void clear() {
    key_map_.clear();
  }
  KeyMap& all() {
    return key_map_;
  }

 private:
  KeyMap key_map_;
};

extern LiveKeys live_keys;

//for (Key key : live_keys.all()) {}

} // namespace kaleidoscope

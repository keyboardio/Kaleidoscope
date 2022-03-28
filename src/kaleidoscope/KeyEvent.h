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

#include <stdint.h>  // for uint8_t, int8_t

#include "kaleidoscope/KeyAddr.h"   // for KeyAddr
#include "kaleidoscope/key_defs.h"  // for Key_Undefined, Key

namespace kaleidoscope {

// It's important that this is a signed integer, not unsigned.
typedef int8_t KeyEventId;

struct KeyEvent {

 public:
  // Constructor for plugin use when regenerating an event with specific ID:
  KeyEvent(KeyAddr addr, uint8_t state, Key key = Key_Undefined, KeyEventId id = last_id_)
    : addr(addr), state(state), key(key), id_(id) {}

  KeyEvent()
    : id_(last_id_) {}

  // For use by keyscanner creating a new event from a physical keyswitch toggle
  // on or off.
  static KeyEvent next(KeyAddr addr, uint8_t state) {
    return KeyEvent(addr, state, Key_Undefined, ++last_id_);
  }

  KeyEventId id() const {
    return id_;
  }
  void swapId(KeyEvent &other) {
    KeyEventId tmp_id = id_;
    id_               = other.id_;
    other.id_         = tmp_id;
  }

  KeyAddr addr  = KeyAddr::none();
  uint8_t state = 0;
  Key key       = Key_Undefined;

 private:
  // serial number of the event:
  static KeyEventId last_id_;
  KeyEventId id_;
};

}  // namespace kaleidoscope

typedef kaleidoscope::KeyEventId KeyEventId;
typedef kaleidoscope::KeyEvent KeyEvent;

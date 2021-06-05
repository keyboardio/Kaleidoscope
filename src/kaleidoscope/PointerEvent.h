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

#include <stdint.h>                 // for uint8_t, int8_t

#include "kaleidoscope/key_defs.h"  // for Key, Key_NoKey
#include "kaleidoscope/KeyAddr.h"   // for KeyAddr

namespace kaleidoscope {

typedef int8_t PointerEventId;

struct PointerEvent {

 public:
  // Constructor for plugin use when regenerating an event with specific ID:
  PointerEvent(KeyAddr addr, int8_t x, int8_t y, int8_t h, int8_t v,
               Key key = Key_NoKey, PointerEventId id = last_id_)
    : addr(addr), x(x), y(y), h(h), v(v), key(key), id_(id) {}

  PointerEvent() : id_(last_id_) {}

  // For use by sensor creating a new event from a physical sensor event.
  static PointerEvent next(KeyAddr addr, int8_t x, int8_t y, int8_t h, int8_t v) {
    return PointerEvent(addr, x, y, h, v, Key_NoKey, ++last_id_);
  }

  PointerEventId id() const {
    return id_;
  }
  void swapId(PointerEvent &other) {
    PointerEventId tmp_id = id_;
    id_ = other.id_;
    other.id_ = tmp_id;
  }

  KeyAddr addr = KeyAddr::none();
  int8_t x = 0;
  int8_t y = 0;
  int8_t h = 0;
  int8_t v = 0;
  Key key = Key_NoKey;

 private:
  // serial number of the event:
  static PointerEventId last_id_;
  PointerEventId id_;
};

}  // namespace kaleidoscope

typedef kaleidoscope::PointerEventId PointerEventId;
typedef kaleidoscope::PointerEvent PointerEvent;

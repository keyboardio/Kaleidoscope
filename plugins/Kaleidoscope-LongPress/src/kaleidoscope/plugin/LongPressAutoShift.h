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

namespace kaleidoscope {
namespace plugin {
namespace longpress {

// ---------------------------------------------------------------------------
// Inner class for `Key` categories that can be configured to be auto-shifted
// by long-pressing.  Most of this class is purely internal, but user code
// that enables or disables these auto-shift categories might use the
// following as constants:
//
// - `longpress::AutoShiftCategories::noKeys()`
// - `longpress::AutoShiftCategories::letterKeys()`
// - `longpress::AutoShiftCategories::numberKeys()`
// - `longpress::AutoShiftCategories::symbolKeys()`
// - `longpress::AutoShiftCategories::arrowKeys()`
// - `longpress::AutoShiftCategories::functionKeys()`
// - `longpress::AutoShiftCategories::printableKeys()`
// - `longpress::AutoShiftCategories::allKeys()`
//
// The first two ("letter keys" and "number keys") are self-explanatory.  The
// third ("symbol keys") includes keys that produce symbols other than letters
// and numbers, but not whitespace characters, modifiers, et cetera.  We could
// perhaps add another category for function keys.
//
// As these methods are delegated to in the LongPress plugin itself there
// should be no need to call these directly from client code.
class AutoShiftCategories {
 private:
  // raw bitfield data
  uint8_t raw_bits_{0};

  // constants for bits in the `raw_bits_` bitfield
  static constexpr uint8_t NONE      = 0 << 0;
  static constexpr uint8_t LETTERS   = 1 << 0;
  static constexpr uint8_t NUMBERS   = 1 << 1;
  static constexpr uint8_t SYMBOLS   = 1 << 2;
  static constexpr uint8_t ARROWS    = 1 << 3;
  static constexpr uint8_t FUNCTIONS = 1 << 4;
  static constexpr uint8_t ALL       = 1 << 7;

 public:
  // Basic un-checked constructor
  explicit constexpr AutoShiftCategories(uint8_t raw_bits)
    : raw_bits_(raw_bits) {}

  static constexpr AutoShiftCategories noKeys() {
    return AutoShiftCategories(NONE);
  }
  static constexpr AutoShiftCategories letterKeys() {
    return AutoShiftCategories(LETTERS);
  }
  static constexpr AutoShiftCategories numberKeys() {
    return AutoShiftCategories(NUMBERS);
  }
  static constexpr AutoShiftCategories symbolKeys() {
    return AutoShiftCategories(SYMBOLS);
  }
  static constexpr AutoShiftCategories arrowKeys() {
    return AutoShiftCategories(ARROWS);
  }
  static constexpr AutoShiftCategories functionKeys() {
    return AutoShiftCategories(FUNCTIONS);
  }
  static constexpr AutoShiftCategories printableKeys() {
    return AutoShiftCategories(LETTERS | NUMBERS | SYMBOLS);
  }
  static constexpr AutoShiftCategories allKeys() {
    return AutoShiftCategories(ALL);
  }

  constexpr void set(uint8_t raw_bits) {
    raw_bits_ = raw_bits;
  }
  constexpr void add(AutoShiftCategories categories) {
    this->raw_bits_ |= categories.raw_bits_;
  }
  constexpr void remove(AutoShiftCategories categories) {
    this->raw_bits_ &= ~(categories.raw_bits_);
  }
  constexpr bool contains(AutoShiftCategories categories) const {
    return (this->raw_bits_ & categories.raw_bits_) != 0;
    // More correct test:
    // return (~(this->raw_bits_) & categories.raw_bits_) == 0;
  }

  // Shorthand for combining categories:
  // e.g. `AutoShiftCategories::letterKeys() | AutoShiftCategories::numberKeys()`
  constexpr AutoShiftCategories operator|(AutoShiftCategories other) const {
    return AutoShiftCategories(this->raw_bits_ | other.raw_bits_);
  }

  // A conversion to integer is provided for the sake of interactions with the
  // Focus plugin.
  explicit constexpr operator uint8_t() {
    return raw_bits_;
  }
};
}  // namespace longpress

extern longpress::AutoShiftCategories AutoShiftCategories;

}  // namespace plugin
}  // namespace kaleidoscope

/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2020  Keyboard.io, Inc.
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


#include "kaleidoscope/HIDTables.h"

#include "kaleidoscope/key_defs_keyboard.h"
#include "kaleidoscope/key_defs_sysctl.h"
#include "kaleidoscope/key_defs_consumerctl.h"
#include "kaleidoscope/key_defs_keymaps.h"

#include "kaleidoscope/key_defs_aliases.h"

namespace kaleidoscope {

class Key {

 public:

  typedef uint16_t StorageType;

  Key() = default;

  constexpr Key(uint16_t raw) // NOLINT(runtime/explicit)
    : Key{(uint8_t)(raw & 0x00FF), (uint8_t)(raw >> 8)}
  {}

  constexpr Key(uint8_t key_code, uint8_t flags)
    : keyCode_{key_code},
      flags_{flags}
  {}

  void setFlags(uint8_t new_flags) {
    flags_ = new_flags;
  }

// gcc 7 (Arduino > 1.8.10) has a problem with the layer dimension of
// keymap_linear, as used by keyFromKeymap(...) being undefined.
// Because of this, we disable -Warray_bounds locally to avoid spurious warnings.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
  constexpr const uint8_t &getFlags() const {
    return flags_;
  }

  void setKeyCode(uint8_t key_code) {
    keyCode_ = key_code;
  }
  constexpr const uint8_t &getKeyCode() const {
    return keyCode_;
  }
#pragma GCC diagnostic pop

  void setRaw(uint16_t raw) {
    flags_  = (uint8_t)(raw >> 8);
    keyCode_ = (uint8_t)(raw & 0x00FF);
  }
  constexpr uint16_t getRaw() const {
    return (uint16_t)(
             ((uint16_t)flags_ << 8)
             + (uint16_t)keyCode_
           );
  }

  constexpr bool operator==(const StorageType rhs) const {
    return this->getRaw() == rhs;
  }
  constexpr bool operator==(const Key& rhs) const {
    return (this->keyCode_ == rhs.keyCode_)
           && (this->flags_ == rhs.flags_);
  }
  Key& operator=(const StorageType raw) {
    this->setRaw(raw);
    return *this;
  }
  constexpr bool operator!=(const Key& rhs) const {
    return !(*this == rhs);
  }
  constexpr bool operator>=(const StorageType raw) const {
    return this->getRaw() >= raw;
  }
  constexpr bool operator<=(const StorageType raw) const {
    return this->getRaw() <= raw;
  }
  constexpr bool operator>(const StorageType raw) const {
    return this->getRaw() > raw;
  }
  constexpr bool operator<(const StorageType raw) const {
    return this->getRaw() < raw;
  }
  constexpr bool operator>=(const Key& other) const {
    return this->getRaw() >= other.getRaw();
  }
  constexpr bool operator<=(const Key& other) const {
    return this->getRaw() <= other.getRaw();
  }
  constexpr bool operator>(const Key& other) const {
    return this->getRaw() > other.getRaw();
  }
  constexpr bool operator<(const Key& other) const {
    return this->getRaw() < other.getRaw();
  }

  Key readFromProgmem() const {
    return Key{pgm_read_byte(&(this->getKeyCode())),
               pgm_read_byte(&(this->getFlags()))};
  }

 private:
  uint8_t keyCode_;
  uint8_t flags_;

};

static_assert(sizeof(Key) == 2, "sizeof(Key) changed");

// Overload this function to define alternative conversions to type Key.
//
constexpr Key convertToKey(Key k) {
  return k;
}

constexpr Key addFlags(Key k, uint8_t add_flags) {
  return Key(k.getKeyCode(), k.getFlags() | add_flags);
}

} // namespace kaleidoscope

// Redefine this macro to enable using alternative char-string to Key
// conversions per layer. This is, howerver, only necessary in rare cases
// (e.g. to use individual host_keymap language maps on
// different layers, see kaleidoscope/host_keymap/host_keymap.h for an example).
//
#define CONVERT_TO_KEY(INPUT) kaleidoscope::convertToKey(INPUT)

// For compatibility reasons make the Key class also available
// in global namespace.
//
typedef kaleidoscope::Key Key;
typedef kaleidoscope::Key Key_;

#define KEY_FLAGS         B00000000
#define CTRL_HELD         B00000001
#define LALT_HELD         B00000010
#define RALT_HELD         B00000100
#define SHIFT_HELD        B00001000
#define GUI_HELD          B00010000

#define SYNTHETIC         B01000000
#define RESERVED          B10000000

#define LCTRL(k)  kaleidoscope::addFlags(CONVERT_TO_KEY(k), CTRL_HELD)
#define LALT(k)   kaleidoscope::addFlags(CONVERT_TO_KEY(k), LALT_HELD)
#define RALT(k)   kaleidoscope::addFlags(CONVERT_TO_KEY(k), RALT_HELD)
#define LSHIFT(k) kaleidoscope::addFlags(CONVERT_TO_KEY(k), SHIFT_HELD)
#define LGUI(k)   kaleidoscope::addFlags(CONVERT_TO_KEY(k), GUI_HELD)

// we assert that synthetic keys can never have keys held, so we reuse the _HELD bits
#define IS_SYSCTL                  B00000001
#define IS_INTERNAL                B00000010
#define SWITCH_TO_KEYMAP           B00000100
#define IS_CONSUMER                B00001000

// HID Usage Types: Because these constants, like the ones above, are
// used in the flags byte of the Key class, they can't overlap any of
// the above bits. Nor can we use `SYNTHETIC` and `RESERVED` to encode
// the HID usage type of a keycode, which leaves us with only two
// bits. Since we don't currently do anything different based on HID
// usage type, these are currently all set to zeroes.
#define HID_TYPE_CA    B00000000
#define HID_TYPE_CL    B00000000
#define HID_TYPE_LC    B00000000
#define HID_TYPE_MC    B00000000
#define HID_TYPE_NARY  B00000000
#define HID_TYPE_OOC   B00000000
#define HID_TYPE_OSC   B00000000
#define HID_TYPE_RTC   B00000000
#define HID_TYPE_SEL   B00000000
#define HID_TYPE_SV    B00000000
// Mask defining the allowed usage type flag bits:
#define HID_TYPE_MASK  B00110000


#define Key_NoKey Key(0, KEY_FLAGS)
#define Key_skip Key(0, KEY_FLAGS)
#define Key_Transparent Key(0xffff)
#define ___ Key_Transparent
#define XXX Key_NoKey

#define KEY_BACKLIGHT_DOWN 0xf1
#define KEY_BACKLIGHT_UP 0xf2
#define Key_BacklightDown Key(KEY_BACKLIGHT_DOWN, KEY_FLAGS)
#define Key_BacklightUp Key(KEY_BACKLIGHT_UP, KEY_FLAGS)
#define KEY_RIGHT_FN2 0xfe
#define Key_RFN2 Key(KEY_RIGHT_FN2, KEY_FLAGS)
#define KEY_LEFT_FN2 0xff
#define Key_LFN2 Key(KEY_LEFT_FN2, KEY_FLAGS)

#define SYSTEM_KEY(code, hid_type) \
  Key(code, SYNTHETIC | IS_SYSCTL | (hid_type & HID_TYPE_MASK))

/* Most Consumer keys are more then 8bit, the highest Consumer hid code
   uses 10bit. By using the 11bit as flag to indicate a consumer keys was activate we can
   use the 10 lsb as the HID Consumer code. If you need to get the keycode of a Consumer key
   use the CONSUMER(key) macro this will return the 10bit keycode.
*/
constexpr uint16_t CONSUMER_KEYCODE_MASK = 0x03FF;
#define CONSUMER(key) (key.getRaw() & CONSUMER_KEYCODE_MASK)
#define CONSUMER_KEY(code, hid_type) \
  Key((code & CONSUMER_KEYCODE_MASK) | \
      ((SYNTHETIC | IS_CONSUMER | (hid_type & HID_TYPE_MASK)) << 8))

namespace kaleidoscope {
constexpr Key bad_keymap_key{0, RESERVED};
}

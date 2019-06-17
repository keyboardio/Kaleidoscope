/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2018  Keyboard.io, Inc.
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

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD
#include "VirtualHID/VirtualHID.h"
#else
#include "KeyboardioHID.h"
#endif

namespace kaleidoscope {

union Key {

  struct {
    uint8_t keyCode;
    uint8_t flags;
  };
  uint16_t raw;

  Key() = default;

  constexpr Key(uint8_t __keyCode, uint8_t __flags)
    : keyCode(__keyCode), flags(__flags) {
  }

  constexpr Key(uint16_t __raw)
    : raw(__raw) {
  }

  constexpr bool operator==(const uint16_t rhs) const {
    return this->raw == rhs;
  }
  constexpr bool operator==(const Key& rhs) const {
    return this->raw == rhs.raw;
  }
  Key& operator=(const uint16_t raw) {
    this->raw = raw;
    return *this;
  }
  constexpr bool operator!=(const Key& rhs) const {
    return !(*this == rhs);
  }
  constexpr bool operator>=(const uint16_t raw) const {
    return this->raw >= raw;
  }
  constexpr bool operator<=(const uint16_t raw) const {
    return this->raw <= raw;
  }
  constexpr bool operator>(const uint16_t raw) const {
    return this->raw > raw;
  }
  constexpr bool operator<(const uint16_t raw) const {
    return this->raw < raw;
  }
  constexpr bool operator>=(const Key& other) const {
    return this->raw >= other.raw;
  }
  constexpr bool operator<=(const Key& other) const {
    return this->raw <= other.raw;
  }
  constexpr bool operator>(const Key& other) const {
    return this->raw > other.raw;
  }
  constexpr bool operator<(const Key& other) const {
    return this->raw < other.raw;
  }
};

} // namespace kaleidoscope

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
// #define IS_MACRO       B00100000  // defined in Kaleidoscope-Macros/src/MacroKeyDefs.h
#define SYNTHETIC         B01000000
#define RESERVED          B10000000

#define LCTRL(k)  Key(k.keyCode, k.flags | CTRL_HELD)
#define LALT(k)   Key(k.keyCode, k.flags | LALT_HELD)
#define RALT(k)   Key(k.keyCode, k.flags | RALT_HELD)
#define LSHIFT(k) Key(k.keyCode, k.flags | SHIFT_HELD)
#define LGUI(k)   Key(k.keyCode, k.flags | GUI_HELD)

// we assert that synthetic keys can never have keys held, so we reuse the _HELD bits
#define IS_SYSCTL                  B00000001
#define IS_INTERNAL                B00000010
#define SWITCH_TO_KEYMAP           B00000100
#define IS_CONSUMER                B00001000

/* HID types we need to encode in the key flags for system and consumer control hid controls
   Each key can only have one, so we don't need to use a bit vector.
   We need to keep the top two bits clear for defining the keys as synthetic
   We need to keep the bottom two bits clear for defining the keys as sysctl / consumerctl
*/

#define HID_TYPE_CL    B00000000
#define HID_TYPE_LC    B00000100
#define HID_TYPE_NARY  B00001000
#define HID_TYPE_OOC   B00001100
#define HID_TYPE_OSC   B00010000
#define HID_TYPE_RTC   B00010100
#define HID_TYPE_SEL   B00011000


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


/* Most Consumer keys are more then 8bit, the highest Consumer hid code
   uses 10bit. By using the 11bit as flag to indicate a consumer keys was activate we can
   use the 10 lsb as the HID Consumer code. If you need to get the keycode of a Consumer key
   use the CONSUMER(key) macro this will return the 10bit keycode.
*/
#define CONSUMER(key) (key.raw & 0x03FF)
#define CONSUMER_KEY(code, flags) Key((code) | ((flags | SYNTHETIC|IS_CONSUMER) << 8))

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

#include <Arduino.h>  // for pgm_read_byte, INPUT
#include <stdint.h>   // for uint8_t, uint16_t

#include "kaleidoscope/HIDTables.h"  // for HID_KEYBOARD_LEFT_CONTROL, HID_KEYBOARD_RIGHT...
// IWYU pragma: begin_exports
#include "kaleidoscope/key_defs/aliases.h"
#include "kaleidoscope/key_defs/consumerctl.h"
#include "kaleidoscope/key_defs/keyboard.h"
#include "kaleidoscope/key_defs/keymaps.h"  // for LAYER_MOVE_OFFSET, LAYER_SHIFT_OFFSET
#include "kaleidoscope/key_defs/sysctl.h"
// IWYU pragma: end_exports

// -----------------------------------------------------------------------------
// Constant keycode values
#define HID_FIRST_KEY               HID_KEYBOARD_NO_EVENT
#define HID_LAST_KEY                HID_KEYPAD_HEXADECIMAL
#define HID_KEYBOARD_FIRST_MODIFIER HID_KEYBOARD_LEFT_CONTROL
#define HID_KEYBOARD_LAST_MODIFIER  HID_KEYBOARD_RIGHT_GUI

// -----------------------------------------------------------------------------
// Constant flags values
#define KEY_FLAGS  0b00000000
#define CTRL_HELD  0b00000001
#define LALT_HELD  0b00000010
#define RALT_HELD  0b00000100
#define SHIFT_HELD 0b00001000
#define GUI_HELD   0b00010000

#define SYNTHETIC  0b01000000
#define RESERVED   0b10000000

// we assert that synthetic keys can never have keys held, so we reuse the _HELD bits
#define IS_SYSCTL        0b00000001
#define IS_INTERNAL      0b00000010
#define SWITCH_TO_KEYMAP 0b00000100
#define IS_CONSUMER      0b00001000

// consumer: 01..1...
// sysctl:   01..0001
// layer:    01000100
// modlayer: 01000110
// macros:   01100000

// HID Usage Types: Because these constants, like the ones above, are
// used in the flags byte of the Key class, they can't overlap any of
// the above bits. Nor can we use `SYNTHETIC` and `RESERVED` to encode
// the HID usage type of a keycode, which leaves us with only two
// bits. Since we don't currently do anything different based on HID
// usage type, these are currently all set to zeroes.
#define HID_TYPE_CA   0b00000000
#define HID_TYPE_CL   0b00000000
#define HID_TYPE_LC   0b00000000
#define HID_TYPE_MC   0b00000000
#define HID_TYPE_NARY 0b00000000
#define HID_TYPE_OOC  0b00000000
#define HID_TYPE_OSC  0b00000000
#define HID_TYPE_RTC  0b00000000
#define HID_TYPE_SEL  0b00000000
#define HID_TYPE_SV   0b00000000
// Mask defining the allowed usage type flag bits:
#define HID_TYPE_MASK 0b00110000


// =============================================================================
namespace kaleidoscope {

class Key {

 public:
  typedef uint16_t StorageType;

  Key() = default;

  constexpr Key(uint16_t raw)  // NOLINT(runtime/explicit)
    : Key{(uint8_t)(raw & 0x00FF), (uint8_t)(raw >> 8)} {}

  constexpr Key(uint8_t key_code, uint8_t flags)
    : keyCode_{key_code},
      flags_{flags} {}

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
    flags_   = (uint8_t)(raw >> 8);
    keyCode_ = (uint8_t)(raw & 0x00FF);
  }
  constexpr uint16_t getRaw() const {
    return (uint16_t)(((uint16_t)flags_ << 8) + (uint16_t)keyCode_);
  }

  constexpr bool operator==(const StorageType rhs) const {
    return this->getRaw() == rhs;
  }
  constexpr bool operator==(const Key &rhs) const {
    return (this->keyCode_ == rhs.keyCode_) && (this->flags_ == rhs.flags_);
  }
  Key &operator=(const StorageType raw) {
    this->setRaw(raw);
    return *this;
  }
  constexpr bool operator!=(const Key &rhs) const {
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
  constexpr bool operator>=(const Key &other) const {
    return this->getRaw() >= other.getRaw();
  }
  constexpr bool operator<=(const Key &other) const {
    return this->getRaw() <= other.getRaw();
  }
  constexpr bool operator>(const Key &other) const {
    return this->getRaw() > other.getRaw();
  }
  constexpr bool operator<(const Key &other) const {
    return this->getRaw() < other.getRaw();
  }

  Key readFromProgmem() const {
    return Key{pgm_read_byte(&(this->getKeyCode())),
               pgm_read_byte(&(this->getFlags()))};
  }

  // ---------------------------------------------------------------------------
  // Builtin Key variant test functions
  //
  // The following functions return `true` if the `key` parameter is of the
  // corresponding HID type (Keyboard, Consumer Control, or System Control).
  constexpr bool isKeyboardKey() const {
    return ((flags_ & (RESERVED | SYNTHETIC)) == 0);
  }
  constexpr bool isSystemControlKey() const {
    return ((flags_ & system_control_mask_) == (SYNTHETIC | IS_SYSCTL));
  }
  constexpr bool isConsumerControlKey() const {
    return ((flags_ & consumer_control_mask_) == (SYNTHETIC | IS_CONSUMER));
  }
  // Not a HID type, but also a builtin `Key` variant.
  constexpr bool isLayerKey() const {
    return (flags_ == (SYNTHETIC | SWITCH_TO_KEYMAP));
  }
  constexpr bool isModLayerKey() const {
    return (flags_ == (SYNTHETIC | SWITCH_TO_KEYMAP | IS_INTERNAL));
  }

  // ---------------------------------------------------------------------------
  // Additional utility functions for builtin `Key` variants
  //
  // An "intentional" Keyboard modifier is any HID Keyboard key that has a
  // keycode byte corresponding to a modifier. This includes combination
  // modifier keys like `LSHIFT(Key_RightAlt)` and `Key_Meh`. It will not match
  // a key with only modifier flags (e.g. `LCTRL(RALT(Key_NoKey))`); this is an
  // intentional feature so that plugins can distinguish between the two.
  constexpr bool __attribute__((always_inline)) isKeyboardModifier() const {
    return (isKeyboardKey() &&
            (keyCode_ >= HID_KEYBOARD_FIRST_MODIFIER &&
             keyCode_ <= HID_KEYBOARD_LAST_MODIFIER));
  }
  // It's important to distinguish between "intentional" modifiers and
  // "incidental" modifiers (non-modifier keys with a modifier flag). The
  // following test matches any Keyboard modifier key that includes a `shift`
  // keycode either in its keycode byte or in its flags. In this way, we treat
  // all of the modifiers in a combination modifier key the same. There should
  // be no difference between `LSHIFT(Key_RightAlt)` and `RALT(Key_LeftShift)`.
  //
  // The `shift` modifiers are special even among the Keyboard modifier keys
  // (and several plugins single them out for this reason), because they are
  // used in a conceptually different way: to get a different symbol in the
  // output. We don't normally think "type `shift`+`1`"; we think "type `!`".
  constexpr bool isKeyboardShift() const {
    return ((isKeyboardModifier() &&
             ((keyCode_ == HID_KEYBOARD_LEFT_SHIFT ||
               keyCode_ == HID_KEYBOARD_RIGHT_SHIFT) ||
              ((flags_ & SHIFT_HELD) != 0))) ||
            (isModLayerKey() &&
             (keyCode_ % 8) % 4 == 1));
  }
  // Layer shift keys are conceptually similar to Keyboard modifier keys in that
  // they are used chorded to change the result of typing those other
  // keys. They're even more similar to `shift` keys. For both reasons, it's
  // worth singling them out.
  constexpr bool __attribute__((always_inline)) isLayerShift() const {
    return ((isLayerKey() &&
             keyCode_ >= LAYER_SHIFT_OFFSET &&
             keyCode_ < LAYER_MOVE_OFFSET) ||
            isModLayerKey());
  }

  constexpr bool isMomentary() const {
    return (isKeyboardModifier() || isLayerShift());
  }

 private:
  uint8_t keyCode_;
  uint8_t flags_;

  // -----------------------------------------------------------------------------
  // Useful constants for the `Key` object's `flags` byte.

  // There are two bits available for Consumer Control & System Control keys to
  // use for storing information about the HID Usage Type. These bits are ones
  // in the following mask.
  static constexpr uint8_t hid_type_mask_ = HID_TYPE_MASK;

  // The bits that must match exactly when checking if a `Key` value corresponds
  // to a System Control keycode.
  static constexpr uint8_t system_control_mask_ = ~hid_type_mask_;

  // The bits that must match exactly when checking if a `Key` value corresponds
  // to a Consumer Control keycode.
  static constexpr uint8_t consumer_control_mask_ = (RESERVED | SYNTHETIC | IS_CONSUMER);
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

// =============================================================================
/// Generate a ModLayer key (unchecked)
constexpr Key modLayerKey(Key modifier, uint8_t layer) {
  uint8_t mod  = modifier.getRaw() - Key_LeftControl.getRaw();
  uint8_t code = mod + (layer * 8);
  return Key(code, SYNTHETIC | SWITCH_TO_KEYMAP | IS_INTERNAL);
}

}  // namespace kaleidoscope

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

#define LCTRL(k)        kaleidoscope::addFlags(CONVERT_TO_KEY(k), CTRL_HELD)
#define LALT(k)         kaleidoscope::addFlags(CONVERT_TO_KEY(k), LALT_HELD)
#define RALT(k)         kaleidoscope::addFlags(CONVERT_TO_KEY(k), RALT_HELD)
#define LSHIFT(k)       kaleidoscope::addFlags(CONVERT_TO_KEY(k), SHIFT_HELD)
#define LGUI(k)         kaleidoscope::addFlags(CONVERT_TO_KEY(k), GUI_HELD)

#define Key_NoKey       Key(0, KEY_FLAGS)
#define Key_skip        Key(0, KEY_FLAGS)
#define Key_Transparent Key(0xffff)
#define ___             Key_Transparent
#define XXX             Key_NoKey

// For entries in the `live_keys[]` array for inactive keys and masked keys,
// respectively:
#define Key_Inactive Key_Transparent
#define Key_Masked   Key_NoKey

// The default value for new events.  Used to signal that a keymap lookup should
// be done.
#define Key_Undefined      Key_Transparent

#define KEY_BACKLIGHT_DOWN 0xf1
#define KEY_BACKLIGHT_UP   0xf2
#define Key_BacklightDown  Key(KEY_BACKLIGHT_DOWN, KEY_FLAGS)
#define Key_BacklightUp    Key(KEY_BACKLIGHT_UP, KEY_FLAGS)
#define KEY_RIGHT_FN2      0xfe
#define Key_RFN2           Key(KEY_RIGHT_FN2, KEY_FLAGS)
#define KEY_LEFT_FN2       0xff
#define Key_LFN2           Key(KEY_LEFT_FN2, KEY_FLAGS)

#define SYSTEM_KEY(code, hid_type) \
  Key(code, SYNTHETIC | IS_SYSCTL | (hid_type & HID_TYPE_MASK))

/* Most Consumer keys are more then 8bit, the highest Consumer hid code
   uses 10bit. By using the 11bit as flag to indicate a consumer keys was activate we can
   use the 10 lsb as the HID Consumer code. If you need to get the keycode of a Consumer key
   use the CONSUMER(key) macro this will return the 10bit keycode.
*/
constexpr uint16_t CONSUMER_KEYCODE_MASK = 0x03FF;
#define CONSUMER(key) (key.getRaw() & CONSUMER_KEYCODE_MASK)
#define CONSUMER_KEY(code, hid_type)   \
  Key((code & CONSUMER_KEYCODE_MASK) | \
      ((SYNTHETIC | IS_CONSUMER | (hid_type & HID_TYPE_MASK)) << 8))

namespace kaleidoscope {
constexpr Key bad_keymap_key{0, RESERVED};
}

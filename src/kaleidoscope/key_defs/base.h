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

#include <stdint.h>  // for uint16_t

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
#define Key_Undefined Key_Transparent

#define LCTRL(k)      kaleidoscope::addFlags(CONVERT_TO_KEY(k), CTRL_HELD)
#define LALT(k)       kaleidoscope::addFlags(CONVERT_TO_KEY(k), LALT_HELD)
#define RALT(k)       kaleidoscope::addFlags(CONVERT_TO_KEY(k), RALT_HELD)
#define LSHIFT(k)     kaleidoscope::addFlags(CONVERT_TO_KEY(k), SHIFT_HELD)
#define LGUI(k)       kaleidoscope::addFlags(CONVERT_TO_KEY(k), GUI_HELD)

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

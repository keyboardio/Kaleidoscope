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

#include "Kaleidoscope.h"

#include "kaleidoscope/host_keymap/ascii/us_en.h"
#include "kaleidoscope/host_keymap/unicode/us_en.h"

USE_HOST_KEYMAP(ascii, us_en)
USE_HOST_KEYMAP(unicode, us_en)

#define KEY_ASSERTION_(KEY_DEF, TEST_KEY, UC)                                  \
   static_assert(                                                              \
      kaleidoscope::convertToKey(UC##KEY_DEF) == TEST_KEY,                     \
      "host_keymap assertion failed: " #UC #KEY_DEF " != " #TEST_KEY           \
   );

#define KEY_ASSERTION(KEY_DEF, TEST_KEY)                                       \
   KEY_ASSERTION_(KEY_DEF, TEST_KEY, )  /* ascii */                            \
   KEY_ASSERTION_(KEY_DEF, TEST_KEY, L) /* unicode */

namespace kaleidoscope {
namespace host_keymap {

// The following can be used to generate compile time errors that
// allow to determine the key_code/flags value of a key for debugging
// purposes.
//
// template<int _value>
// struct _{ constexpr operator char() { return _value + 256; } }; //always overflow
//
// constexpr char key_code = _<kaleidoscope::convertToKey("c+#").getKeyCode()>{};
// constexpr char flags = _<kaleidoscope::convertToKey("c+#").getFlags()>{};

KEY_ASSERTION('\\', Key_Backslash)
KEY_ASSERTION('#', LSHIFT(Key_3))
KEY_ASSERTION('+', LSHIFT(Key_Equals))
KEY_ASSERTION(' ', Key_Space)
KEY_ASSERTION('\t', Key_Tab)
KEY_ASSERTION('\n', Key_Enter)
KEY_ASSERTION('a', Key_A)
KEY_ASSERTION(" a", Key_A)
KEY_ASSERTION("a ", Key_A)
KEY_ASSERTION("ca", LCTRL(Key_A))
KEY_ASSERTION("mca", LGUI(LCTRL(Key_A)))
KEY_ASSERTION("c+a", LCTRL(Key_A))
KEY_ASSERTION(" c+a", LCTRL(Key_A))
KEY_ASSERTION("c +a", LCTRL(Key_A))
KEY_ASSERTION("c+ a", LCTRL(Key_A))
KEY_ASSERTION("c+a ", LCTRL(Key_A))

KEY_ASSERTION("", bad_keymap_key)
KEY_ASSERTION("c+#", bad_keymap_key)
KEY_ASSERTION("# ", Key_Space)
KEY_ASSERTION("c+# ", LCTRL(Key_Space))
KEY_ASSERTION("##", LSHIFT(Key_3))
KEY_ASSERTION("c+##", LCTRL(LSHIFT(Key_3)))
KEY_ASSERTION("c+#+", LCTRL(LSHIFT(Key_Equals)))
KEY_ASSERTION("#", LSHIFT(Key_3)) // If only one key is specified,
// we accept the escape char as normal ascii char
KEY_ASSERTION_(L"⇧", Key_LeftShift,)
KEY_ASSERTION_(L'⇧', Key_LeftShift,)
KEY_ASSERTION_(L"r⇧", Key_RightShift,)
KEY_ASSERTION_(L"rr⇧", Key_LeftShift,)

} // namespace host_keymap
} // namespace kaleidoscope

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

#include "kaleidoscope/host_keymap/host_keymap.h"

#define _HOST_KEYMAP_CONVERT_NON_PRINTABLE_ENUM_CLASS(NAME, KEY)               \
   (linux::XKBKeySym::NAME == np) ? (KEY) :

#define HOST_KEYMAP_NON_PRINTABLE_CONVERTER(KEY_TRANSLATION)                   \
                                                                               \
   constexpr Key convertToKey(XKBKeySym np) {                                  \
      return KEY_TRANSLATION(_HOST_KEYMAP_CONVERT_NON_PRINTABLE_ENUM_CLASS)    \
             bad_keymap_key;                                                   \
   }

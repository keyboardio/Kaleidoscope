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

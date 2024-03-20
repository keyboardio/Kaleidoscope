/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2024  Keyboard.io, Inc.
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

// These legacy keycodes are defined for compatibility with existing ancient
// keymaps. They should not be used in new keymaps.
// We should formally remove them.

#define KEY_BACKLIGHT_DOWN 0xf1
#define KEY_BACKLIGHT_UP   0xf2
#define Key_BacklightDown  Key(KEY_BACKLIGHT_DOWN, KEY_FLAGS)
#define Key_BacklightUp    Key(KEY_BACKLIGHT_UP, KEY_FLAGS)
#define KEY_RIGHT_FN2      0xfe
#define Key_RFN2           Key(KEY_RIGHT_FN2, KEY_FLAGS)
#define KEY_LEFT_FN2       0xff
#define Key_LFN2           Key(KEY_LEFT_FN2, KEY_FLAGS)

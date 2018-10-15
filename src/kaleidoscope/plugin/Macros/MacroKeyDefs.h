/* Kaleidoscope-Macros - Macro keys for Kaleidoscope.
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
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

#define IS_MACRO       B00100000

#define M(n)           (Key){ n, KEY_FLAGS|SYNTHETIC|IS_MACRO }
#define Key_macroKey1  M(1)
#define Key_macroKey2  M(2)
#define Key_macroKey3  M(3)
#define Key_macroKey4  M(4)
#define Key_macroKey5  M(5)
#define Key_macroKey6  M(6)
#define Key_macroKey7  M(7)
#define Key_macroKey8  M(8)
#define Key_macroKey9  M(9)
#define Key_macroKey10 M(10)

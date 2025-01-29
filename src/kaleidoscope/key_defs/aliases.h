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

// This file contains aliases for Kaleidoscope's HID key definitions.
//
// These aliases are provided as a convenience for end users Our goal is to
// offer up macros that fit what users guess the macros might be called, while
// not polluting the main tables of aliases.
//
// The first group defined here provide short alternate names for a variety
// of keys.

#define Key_Space          Key_Spacebar
#define Key_LBracket       Key_LeftBracket
#define Key_LArrow         Key_LeftArrow
#define Key_LCtrl          Key_LeftControl
#define Key_LShift         Key_LeftShift
#define Key_LAlt           Key_LeftAlt
#define Key_LGui           Key_LeftGui
#define Key_RBracket       Key_RightBracket
#define Key_RArrow         Key_RightArrow
#define Key_RCtrl          Key_RightControl
#define Key_RShift         Key_RightShift
#define Key_RAlt           Key_RightAlt
#define Key_RGui           Key_RightGui
#define Key_Esc            Key_Escape

#define Key_LSquareBracket Key_LeftBracket
#define Key_RSquareBracket Key_RightBracket

// To match Key_UpArrow
#define Key_DnArrow           Key_DownArrow


#define Key_LeftParen         LSHIFT(Key_9)
#define Key_RightParen        LSHIFT(Key_0)
#define Key_LeftCurlyBracket  LSHIFT(Key_LeftBracket)
#define Key_RightCurlyBracket LSHIFT(Key_RightBracket)

#define Key_Pipe              LSHIFT(Key_Backslash)


#define Key_Meh               LCTRL(LSHIFT(Key_LeftAlt))
#define Key_Hyper             MEH(Key_LeftGui)


// Apple's "globe" key (Consumer Control version).  This key will bring up the
// emoji tool on MacOS, just like tapping on the `fn` key on an Apple keyboard.
// Unfortunately, it does not act as the `fn` "modifier" when held, however;
// that key has no equivalent in the USB HID tables, so there's no way for a
// third-party keyboard to emulate it.  This alias is included here because it
// doesn't have any obvious relationship to the "next keyboard layout select"
// canonical name in the HID tables.
#define Key_Apple_Globe Consumer_AC_NextKeyboardLayoutSelect

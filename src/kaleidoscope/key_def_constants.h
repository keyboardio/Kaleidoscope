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

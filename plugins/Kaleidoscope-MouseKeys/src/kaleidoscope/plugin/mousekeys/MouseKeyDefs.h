/* Kaleidoscope-MouseKeys - Mouse keys for Kaleidoscope.
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

#define IS_MOUSE_KEY 0b00010000

// Synthetic, not internal

#define KEY_MOUSE_UP       0b0000001
#define KEY_MOUSE_DOWN     0b0000010
#define KEY_MOUSE_LEFT     0b0000100
#define KEY_MOUSE_RIGHT    0b0001000
#define KEY_MOUSE_WHEEL    0b0010000
#define KEY_MOUSE_WARP     0b0100000
#define KEY_MOUSE_WARP_END 0b1000000
// all buttons end warp. also, we're out of bits
#define KEY_MOUSE_BUTTON  0b1000000


#define Key_mouseWarpNW   Key(KEY_MOUSE_WARP | KEY_MOUSE_UP | KEY_MOUSE_LEFT, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseWarpN    Key(KEY_MOUSE_WARP | KEY_MOUSE_UP, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseWarpNE   Key(KEY_MOUSE_WARP | KEY_MOUSE_UP | KEY_MOUSE_RIGHT, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseWarpW    Key(KEY_MOUSE_WARP | KEY_MOUSE_LEFT, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseWarpIn   Key(KEY_MOUSE_WARP | KEY_MOUSE_UP | KEY_MOUSE_DOWN, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseWarpE    Key(KEY_MOUSE_WARP | KEY_MOUSE_RIGHT, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseWarpSW   Key(KEY_MOUSE_WARP | KEY_MOUSE_DOWN | KEY_MOUSE_LEFT, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseWarpS    Key(KEY_MOUSE_WARP | KEY_MOUSE_DOWN, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseWarpSE   Key(KEY_MOUSE_WARP | KEY_MOUSE_DOWN | KEY_MOUSE_RIGHT, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseWarpEnd  Key(KEY_MOUSE_WARP | KEY_MOUSE_WARP_END, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)


#define Key_mouseUpL      Key(KEY_MOUSE_UP | KEY_MOUSE_LEFT, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseUp       Key(KEY_MOUSE_UP, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseUpR      Key(KEY_MOUSE_UP | KEY_MOUSE_RIGHT, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseL        Key(KEY_MOUSE_LEFT, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseR        Key(KEY_MOUSE_RIGHT, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseDnL      Key(KEY_MOUSE_DOWN | KEY_MOUSE_LEFT, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseDn       Key(KEY_MOUSE_DOWN, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseDnR      Key(KEY_MOUSE_DOWN | KEY_MOUSE_RIGHT, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseScrollUp Key(KEY_MOUSE_WHEEL | KEY_MOUSE_UP, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseScrollDn Key(KEY_MOUSE_WHEEL | KEY_MOUSE_DOWN, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseScrollL  Key(KEY_MOUSE_WHEEL | KEY_MOUSE_LEFT, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseScrollR  Key(KEY_MOUSE_WHEEL | KEY_MOUSE_RIGHT, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)


// The MOUSE_xxx button definitions come from our HID driver
// If the HID driver doesn't define them, don't blow up compilation

#ifdef MOUSE_LEFT

#define KEY_MOUSE_BTN_L MOUSE_LEFT    // Synthetic key
#define KEY_MOUSE_BTN_M MOUSE_MIDDLE  // Synthetic key
#define KEY_MOUSE_BTN_R MOUSE_RIGHT   // Synthetic key
#define KEY_MOUSE_BTN_P MOUSE_PREV
#define KEY_MOUSE_BTN_N MOUSE_NEXT
#define Key_mouseBtnL   Key(KEY_MOUSE_BUTTON | KEY_MOUSE_BTN_L, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseBtnM   Key(KEY_MOUSE_BUTTON | KEY_MOUSE_BTN_M, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseBtnR   Key(KEY_MOUSE_BUTTON | KEY_MOUSE_BTN_R, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseBtnP   Key(KEY_MOUSE_BUTTON | KEY_MOUSE_BTN_P, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)
#define Key_mouseBtnN   Key(KEY_MOUSE_BUTTON | KEY_MOUSE_BTN_N, KEY_FLAGS | SYNTHETIC | IS_MOUSE_KEY)

#else

#define Key_mouseBtnL XXX
#define Key_mouseBtnM XXX
#define Key_mouseBtnR XXX
#define Key_mouseBtnP XXX
#define Key_mouseBtnN XXX


#endif

/* -*- mode: c++ -*-
 * Kaleidoscope - A Kaleidoscope example
 * Copyright (C) 2016-2022  Keyboard.io, Inc.
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

#define DEBUG_SERIAL false

#include <Kaleidoscope.h>
#include <Kaleidoscope-MouseKeys.h>

enum {
  PRIMARY,
  MOUSEKEYS,
};

// clang-format off
KEYMAPS(
  [PRIMARY] = KEYMAP_STACKED
  (Key_NoKey,    Key_1, Key_2, Key_3, Key_4, Key_5, Key_NoKey,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,

   Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
   ShiftToLayer(MOUSEKEYS),

   Key_skip,  Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_skip,
   Key_Enter, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
              Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   Key_skip,  Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,

   Key_RightShift, Key_RightAlt, Key_Spacebar, Key_RightControl,
   LockLayer(MOUSEKEYS)
   ),

  [MOUSEKEYS] =  KEYMAP_STACKED
  (___, ___, ___,           ___,           ___,             ___,             ___,
   ___, ___, ___,           ___,           Key_mouseWarpNW, Key_mouseWarpNE, ___,
   ___, ___, ___,           ___,           Key_mouseWarpSW, Key_mouseWarpSE,
   ___, ___, Key_mouseBtnL, Key_mouseBtnM, Key_mouseBtnR,   ___,             ___,
   ___, ___, ___, ___,
   ___,

   ___, ___, ___,         ___,         ___,        ___, ___,
   ___, ___, ___,         Key_mouseUp, ___,        ___, ___,
        ___, Key_mouseUp, Key_mouseDn, Key_mouseR, ___, ___,
   ___, ___, ___,         ___,         ___,        ___, ___,
   ___, ___, ___, ___,
   ___)
)
// clang-format on


KALEIDOSCOPE_INIT_PLUGINS(MouseKeys);

void setup() {
  Kaleidoscope.setup();
  MouseKeys.setSpeedLimit(100);
  MouseKeys.setWarpGridSize(MOUSE_WARP_GRID_2X2);
}

void loop() {
  Kaleidoscope.loop();
}

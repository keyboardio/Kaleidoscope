/* -*- mode: c++ -*-
 * ErgoDox -- A very basic Kaleidoscope example for the ErgoDox
 * Copyright (C) 2018  Keyboard.io, Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "Kaleidoscope.h"

/* *INDENT-OFF* */
KEYMAPS(
  [0] = KEYMAP_STACKED
  (
      // left hand
      Key_Equals,      Key_1,         Key_2,     Key_3,         Key_4,   Key_5,   Key_LeftArrow,
      Key_Delete,      Key_Q,         Key_W,     Key_E,         Key_R,   Key_T,   Key_NoKey,
      Key_Backspace,   Key_A,         Key_S,     Key_D,         Key_F,   Key_G,
      Key_LeftShift,   Key_Z,         Key_X,     Key_C,         Key_V,   Key_B,   Key_LeftControl,
      Key_Backtick,    Key_Quote,     Key_NoKey, Key_LeftArrow, Key_RightArrow,

      Key_NoKey, Key_LeftGui,
      Key_Home,
      Key_Space, Key_Backspace, Key_End,

      // right hand
      Key_RightArrow,   Key_6,         Key_7,  Key_8,     Key_9,      Key_0,         Key_Minus,
      Key_NoKey,        Key_Y,         Key_U,  Key_I,     Key_O,      Key_P,         Key_Backslash,
                        Key_H,         Key_J,  Key_K,     Key_L,      Key_Semicolon, Key_Quote,
      Key_RightControl, Key_N,         Key_M,  Key_Comma, Key_Period, Key_Slash,     Key_RightShift,
      Key_UpArrow,      Key_DownArrow, XXX,    XXX,          Key_NoKey,

      Key_LeftAlt,  Key_Esc,
      Key_PageUp,
      Key_PageDown, Key_Tab, Key_Enter
  ),
)
/* *INDENT-ON* */

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

/* -*- mode: c++ -*-
 * Kaleidoscope-TopsyTurvy -- Turn the effect of Shift upside down for certain keys
 * Copyright (C) 2017, 2018  Keyboard.io, Inc
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

#include <Kaleidoscope.h>
#include <Kaleidoscope-TopsyTurvy.h>

// clang-format off
KEYMAPS(
  [0] = KEYMAP_STACKED
  (
    Key_NoKey,    TOPSY(1), TOPSY(2), TOPSY(3), TOPSY(4), TOPSY(5), Key_NoKey,
    Key_Backtick, Key_Q,    Key_W,    Key_E,    Key_R,    Key_T, Key_Tab,
    Key_PageUp,   Key_A,    Key_S,    Key_D,    Key_F,    Key_G,
    Key_PageDown, Key_Z,    Key_X,    Key_C,    Key_V,    Key_B, Key_Escape,

    Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
    Key_skip,

    Key_skip,  TOPSY(6), TOPSY(7), TOPSY(8),     TOPSY(9),      TOPSY(0),  Key_skip,
    Key_Enter, Key_Y,    Key_U,    Key_I,        Key_O,         Key_P,     Key_Equals,
    Key_H,     Key_J,    Key_K,    Key_L,        Key_Semicolon, Key_Quote,
    Key_skip,  Key_N,    Key_M,    Key_Comma,    Key_Period,    Key_Slash, Key_Minus,

    Key_RightShift, Key_RightAlt, Key_Spacebar, Key_RightControl,
    Key_skip),
)
// clang-format on

KALEIDOSCOPE_INIT_PLUGINS(TopsyTurvy);

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

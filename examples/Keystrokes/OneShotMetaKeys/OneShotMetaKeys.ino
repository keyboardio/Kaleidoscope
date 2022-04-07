/* -*- mode: c++ -*-
 * Kaleidoscope-OneShotMetaKeys -- Special OneShot keys
 * Copyright (C) 2021  Keyboard.io, Inc.
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
#include <Kaleidoscope-Macros.h>
#include <Kaleidoscope-OneShot.h>
#include <Kaleidoscope-OneShotMetaKeys.h>

// Macros
enum {
  TOGGLE_ONESHOT,
};

// clang-format off
KEYMAPS(
  [0] = KEYMAP_STACKED
  (
    M(TOGGLE_ONESHOT), Key_1, Key_2, Key_3, Key_4, Key_5, OneShot_MetaStickyKey,
    Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
    Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
    Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,

    OSM(LeftControl), Key_Backspace, OSM(LeftGui), OSM(LeftShift),
    Key_Meh,

    OneShot_ActiveStickyKey, Key_6, Key_7, Key_8, Key_9, Key_0,    ___,
    Key_Enter, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
               Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
    ___,       Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,

    Key_RightShift, Key_RightAlt, Key_Spacebar, Key_RightControl,
    OSL(1)),

  [1] = KEYMAP_STACKED
  (
    ___, ___, ___, ___, ___, ___, ___,
    ___, ___, ___, ___, ___, ___, ___,
    ___, ___, ___, ___, ___, ___,
    ___, ___, ___, ___, ___, ___, ___,

    ___, ___, ___, ___,
    ___,

    ___, ___,         ___,         ___,        ___,                ___, ___,
    ___, ___,         ___,         ___,        ___,                ___, ___,
         Key_UpArrow, Key_DownArrow, Key_LeftArrow, Key_RightArrow,___, ___,
    ___, ___,         ___,         ___,        ___,                ___, ___,

    ___, ___, ___, ___,
    ___),
)
// clang-format on

void macroToggleOneShot() {
  OneShot.toggleAutoOneShot();
}

const macro_t *macroAction(uint8_t macro_id, KeyEvent &event) {
  if (macro_id == TOGGLE_ONESHOT) {
    macroToggleOneShot();
  }

  return MACRO_NONE;
}

KALEIDOSCOPE_INIT_PLUGINS(OneShot, OneShotMetaKeys, Macros);

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

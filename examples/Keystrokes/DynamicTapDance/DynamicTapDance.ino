/* -*- mode: c++ -*-
 * DynamicTapDance -- Dynamic TapDance support for Kaleidoscope
 * Copyright (C) 2019  Keyboard.io, Inc
 * Copyright (C) 2019  Dygma Lab S.L.
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
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-FocusSerial.h>
#include <Kaleidoscope-TapDance.h>
#include <Kaleidoscope-DynamicTapDance.h>

// clang-format off
KEYMAPS(
  [0] = KEYMAP_STACKED
  (
    Key_NoKey,    Key_1, Key_2, Key_3, Key_4, Key_5, Key_NoKey,
    Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
    Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
    Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,

    Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
    TD(0),

    Key_skip,  Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_skip,
    Key_Enter, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
               Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
    Key_skip,  Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,

    Key_RightShift, Key_RightAlt, Key_Spacebar, TD(2),
    TD(1)),
)
// clang-format on

enum {
  TD_TAB_ESC,
  TD_LAST
};

void tapDanceAction(uint8_t tap_dance_index, KeyAddr key_addr, uint8_t tap_count, kaleidoscope::plugin::TapDance::ActionType tap_dance_action) {
  switch (tap_dance_index) {
  case TD_TAB_ESC:
    return tapDanceActionKeys(tap_count, tap_dance_action, Key_A, Key_B);
  default:
    DynamicTapDance.dance(tap_dance_index, key_addr, tap_count, tap_dance_action);
  }
}

KALEIDOSCOPE_INIT_PLUGINS(EEPROMSettings,
                          Focus,
                          TapDance,
                          DynamicTapDance);

void setup() {
  Kaleidoscope.setup();
  DynamicTapDance.setup(TD_LAST, 32);
}

void loop() {
  Kaleidoscope.loop();
}

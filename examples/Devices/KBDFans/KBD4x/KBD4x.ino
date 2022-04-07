/* -*- mode: c++ -*-
 * KBD4x -- A very basic Kaleidoscope example for the KBDFans KBD4x keyboard
 * Copyright (C) 2019  Keyboard.io, Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTabILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "Kaleidoscope.h"
#include "Kaleidoscope-Macros.h"

enum {
  _QWERTY,
  _FN
};

enum {
  RESET
};

#define MO(n) ShiftToLayer(n)

// clang-format off
KEYMAPS(

/* Qwerty
 * ,-------------------------------------------------------------------------.
 * | Esc  |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  | Bksp |
 * |------+-----+-----+-----+-----+-----------+-----+-----+-----+-----+------|
 * | Tab  |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  "   |
 * |------+-----+-----+-----+-----+-----|-----+-----+-----+-----+-----+------|
 * | Shift|  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  | Up  |Enter |
 * |------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+------|
 * | Ctrl | GUI |  1  |  2  |  3  |   Space   |  FN |  /  | Lft | Dn  |Right |
 * `-------------------------------------------------------------------------'
 */

[_QWERTY] = KEYMAP(
   Key_Escape      ,Key_Q       ,Key_W ,Key_E ,Key_R ,Key_T ,Key_Y ,Key_U   ,Key_I     ,Key_O         ,Key_P         ,Key_Backspace
  ,Key_Tab         ,Key_A       ,Key_S ,Key_D ,Key_F ,Key_G ,Key_H ,Key_J   ,Key_K     ,Key_L         ,Key_Semicolon ,Key_Quote
  ,Key_LeftShift   ,Key_Z       ,Key_X ,Key_C ,Key_V ,Key_B ,Key_N ,Key_M   ,Key_Comma ,Key_Period    ,Key_UpArrow   ,Key_Enter
  ,Key_LeftControl ,Key_LeftGui ,Key_1 ,Key_2 ,Key_3   ,Key_Space  ,MO(_FN) ,Key_Slash ,Key_LeftArrow ,Key_DownArrow ,Key_RightArrow
),

/* Fn
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |      |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      | RST  |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_FN] = KEYMAP(
   Key_Backtick ,Key_1 ,Key_2 ,Key_3 ,Key_4 ,Key_5 ,Key_6 ,Key_7 ,Key_8 ,Key_9 ,Key_0 ,___
  ,___          ,___   ,___   ,___   ,___   ,___   ,___   ,___   ,___   ,___   ,___   ,M(RESET)
  ,___          ,___   ,___   ,___   ,___   ,___   ,___   ,___   ,___   ,___   ,___   ,___
  ,___          ,___   ,___   ,___   ,___       ,___      ,___   ,___   ,___   ,___   ,___
)
);
// clang-format on

KALEIDOSCOPE_INIT_PLUGINS(Macros);

const macro_t *macroAction(uint8_t macro_id, KeyEvent &event) {
  switch (macro_id) {
  case RESET:
    if (keyToggledOn(event.state))
      Kaleidoscope.rebootBootloader();
    break;
  default:
    break;
  }

  return MACRO_NONE;
}

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

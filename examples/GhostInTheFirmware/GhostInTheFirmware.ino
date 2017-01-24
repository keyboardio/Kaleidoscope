/* -*- mode: c++ -*-
 * Akela -- Animated Keyboardio Extension Library for Anything
 * Copyright (C) 2017  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Akela-GhostInTheFirmware.h>
#include <Keyboardio-Macros.h>

const Key keymaps[][ROWS][COLS] PROGMEM = {
  [0] = KEYMAP_STACKED
  (
   Key_LEDEffectNext, Key_1, Key_2, Key_3, Key_4, Key_5, M(0),
   Key_Backtick,      Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,        Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDn,        Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Esc,

   Key_LCtrl, Key_Backspace, Key_LGUI, Key_LShift,
   Key_skip,

   Key_skip,  Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_skip,
   Key_Enter, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
              Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   Key_skip,  Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,

   Key_RShift, Key_RAlt, Key_Space, Key_RCtrl,
   Key_skip
   ),
};

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  if (macroIndex == 0 && key_toggled_on (keyState))
    GhostInTheFirmware.activate ();

  return MACRO_NONE;
}

static const Akela::GhostInTheFirmware::GhostKey ghostKeys[] PROGMEM = {
  {0, 0, 10},
  {0, 0, 0}
};

void setup () {
  Keyboardio.setup (KEYMAP_SIZE);

  GhostInTheFirmware.configure (ghostKeys);
  Keyboardio.use (&GhostInTheFirmware, &Macros, NULL);
}

void loop () {
  Keyboardio.loop ();
}

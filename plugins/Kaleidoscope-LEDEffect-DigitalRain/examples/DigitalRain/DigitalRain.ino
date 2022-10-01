// -*- mode: c++ -*-
/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2017-2020  Bart Nagel
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

#include "Kaleidoscope.h"
#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-LEDEffect-DigitalRain.h"

// clang-format off

KEYMAPS(
  [0] = KEYMAP_STACKED
  (XXX,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_LEDEffectNext,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,

   Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
   XXX,

   XXX,          Key_6, Key_7, Key_8,     Key_9,         Key_0,         XXX,
   Key_Enter,    Key_Y, Key_U, Key_I,     Key_O,         Key_P,         Key_Equals,
                 Key_H, Key_J, Key_K,     Key_L,         Key_Semicolon, Key_Quote,
   Key_RightAlt, Key_N, Key_M, Key_Comma, Key_Period,    Key_Slash,     Key_Minus,

   Key_RightShift, Key_LeftAlt, Key_Spacebar, Key_RightControl,
   XXX)
) // KEYMAPS(

// clang-format on

KALEIDOSCOPE_INIT_PLUGINS(
  // LEDControl provides support for other LED modes
  LEDControl,

  // Support for DigitalRain
  LEDDigitalRainEffect);


void setup() {
  Kaleidoscope.setup();
  LEDDigitalRainEffect.activate();
}

void loop() {
  Kaleidoscope.loop();
}

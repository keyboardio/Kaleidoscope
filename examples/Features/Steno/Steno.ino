/* -*- mode: c++ -*-
 * Kaleidoscope-Steno -- Steno protocols for Kaleidoscope
 * Copyright (C) 2017  Joseph Wasson
 * Copyright (C) 2017, 2018  Gergely Nagy
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

#include <Kaleidoscope.h>
#include <Kaleidoscope-Steno.h>

// clang-format off
KEYMAPS(
  [0] = KEYMAP_STACKED
  (Key_NoKey,         Key_1, Key_2, Key_3, Key_4, Key_5, Key_NoKey,
   Key_Backtick,      Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,        Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown,      Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,

   Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
   Key_Keymap1,

   Key_skip,  Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_skip,
   Key_Enter, Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_Equals,
              Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   Key_skip,  Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_Minus,

   Key_RightShift, Key_RightAlt, Key_Spacebar, Key_RightControl,
   Key_Keymap1),

  [1] = KEYMAP_STACKED
  (XXX,    XXX,   XXX,   XXX,   XXX,   XXX,   S(N6),
   XXX,    S(N1), S(N2), S(N3), S(N4), S(N5), S(ST1),
   S(FN),  S(S1), S(TL), S(PL), S(HL), S(ST1),
   S(PWR), S(S2), S(KL), S(WL), S(RL), S(ST2), S(ST2),

   S(RE1), XXX, S(A), S(O),
   ___,

   S(N7),  XXX,    XXX,   XXX,   XXX,   XXX,   XXX,
   S(ST3), S(N8),  S(N9), S(NA), S(NB), S(NC), XXX,
           S(ST3), S(FR), S(PR), S(LR), S(TR), S(DR),
   S(ST4), S(ST4), S(RR), S(BR), S(GR), S(SR), S(ZR),

   S(E), S(U), XXX, S(RE2),
   ___),
)
// clang-format on

KALEIDOSCOPE_INIT_PLUGINS(GeminiPR);

void setup() {
  Kaleidoscope.serialPort().begin(9600);
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

/* -*- mode: c++ -*-
 * Kaleidoscope-PloverHID -- Plover HID protocol support for Kaleidoscope
 * Copyright (C) 2025 Keyboard.io, inc.
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
#include <Kaleidoscope-PloverHID.h>

// Example keymap with Plover HID keys
// This demonstrates a basic stenography layout using the Plover HID protocol

/* *INDENT-OFF* */
KEYMAPS(
  [0] = KEYMAP_STACKED
  (
    // Left hand - standard steno keys
    Key_Escape,   PH(NUM),      PH(S_L),      PH(T_L),      PH(P_L),      PH(H_L),      Key_Backspace,
    Key_Tab,      PH(S_L),      PH(K_L),      PH(W_L),      PH(R_L),      PH(STAR),     Key_Delete,
    Key_LeftControl, PH(S_L),   PH(T_L),      PH(P_L),      PH(H_L),      PH(STAR),
    Key_LeftShift,   Key_Z,     Key_X,        Key_C,        Key_V,        Key_B,        Key_LeftAlt,

    Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
    PH(A),

    // Right hand - standard steno keys  
    Key_F1,       PH(F_R),      PH(P_R),      PH(L_R),      PH(T_R),      PH(D_R),      Key_Escape,
    Key_F2,       PH(STAR),     PH(R_R),      PH(B_R),      PH(G_R),      PH(S_R),      PH(Z_R),
                  PH(STAR),     PH(R_R),      PH(B_R),      PH(G_R),      PH(S_R),      Key_RightControl,
    Key_RightAlt, Key_N,        Key_M,        Key_Comma,    Key_Period,   Key_Slash,    Key_RightShift,

    Key_RightShift, Key_RightAlt, Key_Space, Key_RightControl,
    PH(O)
  ),

  [1] = KEYMAP_STACKED
  (
    // Alternative layer with extended keys
    ___,          PH(X1),       PH(X2),       PH(X3),       PH(X4),       PH(X5),       ___,
    ___,          PH(X6),       PH(X7),       PH(X8),       PH(X9),       PH(X10),      ___,
    ___,          PH(X11),      PH(X12),      PH(X13),      PH(X14),      PH(X15),
    ___,          PH(X16),      PH(X17),      PH(X18),      PH(X19),      PH(X20),      ___,

    ___, ___, ___, ___,
    PH(E),

    ___,          PH(X21),      PH(X22),      PH(X23),      PH(X24),      PH(X25),      ___,
    ___,          PH(X26),      PH(X27),      PH(X28),      PH(X29),      PH(X30),      ___,
                  PH(X31),      PH(X32),      PH(X33),      PH(X34),      PH(X35),      ___,
    ___,          PH(X36),      PH(X37),      PH(X38),      PH(X39),      PH(X40),      ___,

    ___, ___, ___, ___,
    PH(U)
  )
)
/* *INDENT-ON* */

KALEIDOSCOPE_INIT_PLUGINS(PloverHID);

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

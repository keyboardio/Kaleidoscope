/* -*- mode: c++ -*-
 * Copyright (C) 2020  Keyboard.io, Inc.
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

// *INDENT-OFF*
KEYMAPS(
  [0] = KEYMAP_STACKED
  (
      M(0), M(1), M(255), M(2), M(3), ___, ___,
      Key_X, ___, ___, ___, ___, ___, ___,
      ___, ___, ___, ___, ___, ___,
      ___, ___, ___, ___, ___, ___, ___,

      ___, ___, ___, ___,
      ___,

      ___, ___, ___, ___, ___, ___, ___,
      ___, ___, ___, ___, ___, ___, ___,
           ___, ___, ___, ___, ___, ___,
      ___, ___, ___, ___, ___, ___, ___,

      ___, ___, ___, ___,
      ___
   ),
)
// *INDENT-ON*

const macro_t *macroAction(uint8_t macro_id, KeyEvent &event) {
  if (keyToggledOn(event.state)) {
    switch (macro_id) {
    case 0:
      Macros.type(PSTR("a"));
      break;
    case 1:
      Macros.type(PSTR("abc"));
      break;
    case 2:
      return MACRO(D(A), T(C), U(A), T(B));
    case 3:
      return MACRO(D(A), D(B));
    case 255:
      Macros.type(PSTR("c"));
      break;
    }
  }
  return MACRO_NONE;
}

// Use Redial
KALEIDOSCOPE_INIT_PLUGINS(Macros);

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

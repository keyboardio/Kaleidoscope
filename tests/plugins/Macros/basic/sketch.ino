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
      M(0), M(1), M(255), ___, ___, ___, ___,
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

const macro_t *macroAction(uint8_t index, uint8_t key_state) {
  if (keyToggledOn(key_state)) {
    switch (index) {
    case 0:
      Kaleidoscope.hid().keyboard().pressKey(Key_A);
      break;
    case 1:
      Kaleidoscope.hid().keyboard().pressKey(Key_B);
      break;
    case 255:
      Kaleidoscope.hid().keyboard().pressKey(Key_C);
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

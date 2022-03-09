/* -*- mode: c++ -*-
 * Copyright (C) 2022  Keyboard.io, Inc.
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
#include <Kaleidoscope-MouseKeys.h>

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        Key_mouseUp, Key_mouseDn, Key_mouseL, Key_mouseR, ___, ___, ___,
        Key_mouseScrollUp, Key_mouseScrollDn, Key_mouseScrollL, Key_mouseScrollR, ___, ___, ___,
        Key_mouseBtnL, Key_mouseBtnM, Key_mouseBtnR, ___, ___, ___,
        M(0), M(1), ___, ___, ___, ___, ___,
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
      Macros.tap(Key_mouseBtnR);
      break;
    case 1:
      Macros.press(Key_mouseScrollUp);
      Macros.release(Key_mouseScrollUp);
      break;
    default:
      break;
    }
  }
  return MACRO_NONE;
}

KALEIDOSCOPE_INIT_PLUGINS(Macros, MouseKeys);

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

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
#include <Kaleidoscope-MouseKeys.h>

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        Key_mouseUp, Key_mouseDn, Key_mouseL, Key_mouseR, ___, ___, ___,
        Key_mouseScrollUp, Key_mouseScrollDn, Key_mouseScrollL, Key_mouseScrollR, ___, ___, ___,
        Key_mouseBtnL, Key_mouseBtnM, Key_mouseBtnR, ___, ___, ___,
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

KALEIDOSCOPE_INIT_PLUGINS(MouseKeys);

void setup() {
  Kaleidoscope.setup();

  MouseKeys.setCursorAccelDuration(200);
}

void loop() {
  Kaleidoscope.loop();
}

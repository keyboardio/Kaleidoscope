/* -*- mode: c++ -*-
 * Copyright (C) 2021  Keyboard.io, Inc.
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
#include <Kaleidoscope-CharShift.h>

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        Key_LeftShift, Key_RightShift, ___, ___, ___, ___, ___,
        Key_X, LSHIFT(Key_Y), ___, ___, ___, ___, ___,
        CS(0), CS(1), CS(2), CS(3), ___, ___,
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

KALEIDOSCOPE_INIT_PLUGINS(CharShift);

void setup() {
  Kaleidoscope.setup();
  CS_KEYS(
    kaleidoscope::plugin::CharShift::KeyPair(Key_A, Key_B),
    kaleidoscope::plugin::CharShift::KeyPair(Key_C, LSHIFT(Key_D)),
    kaleidoscope::plugin::CharShift::KeyPair(LSHIFT(Key_E), Key_F),
    kaleidoscope::plugin::CharShift::KeyPair(LSHIFT(Key_G), LSHIFT(Key_H)),
  );
}

void loop() {
  Kaleidoscope.loop();
}

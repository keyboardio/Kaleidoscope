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
#include <Kaleidoscope-LongPress.h>

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        Key_LeftShift, Key_RightShift, ___, ___, ___, ___, ___,
        Key_A, Key_B, Key_B, ___, ___, ___, ___,
        Key_1, ___, ___, ___, ___, ___,
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

KALEIDOSCOPE_INIT_PLUGINS(LongPress);

void setup() {
  Kaleidoscope.setup();
  LongPress.setTimeout(20);
  LongPress.setAutoshiftEnabled(LongPress.letterKeys());

  LONGPRESS(
    // ATTENTION! The order matters here! The first matching entry wins.

    // overrides the long-press for a logical Key (in the next line)
    kaleidoscope::plugin::LongPressKey(kaleidoscope::plugin::longpress::ALL_LAYERS, KeyAddr(1, 2), Key_Z),
    // overrides the auto-shift functionality
    kaleidoscope::plugin::LongPressKey(kaleidoscope::plugin::longpress::ALL_LAYERS, Key_B,         Key_Y),
  )
}

void loop() {
  Kaleidoscope.loop();
}

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
#include <Kaleidoscope.h>

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        Key_LeftShift, Key_RightShift, ___, ___, ___, ___, ShiftToLayer(1),
        Key_A, Key_B, Key_C, Key_D, ___, ___, ___,
        Key_E, ___, ___, ___, ___, ___,
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

    [1] = KEYMAP_STACKED
    (
        ___, ___, ___, ___, ___, ___, ___,
        Key_A, Key_B, Key_C, Key_D, ___, ___, ___,
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

KALEIDOSCOPE_INIT_PLUGINS(LongPress);

void setup() {
  Kaleidoscope.setup();
  LongPress.setTimeout(20);

  // no auto-shift enabled

  LONGPRESS(
    // Key at 1,0 should produce a Z on long press on all layers
    kaleidoscope::plugin::LongPressKey(kaleidoscope::plugin::longpress::ALL_LAYERS,
                                       KeyAddr(1, 0), Key_Z),
    // Keys generating a B should produce a Y on long press on all layers
    kaleidoscope::plugin::LongPressKey(kaleidoscope::plugin::longpress::ALL_LAYERS,
                                       Key_B,         Key_Y),
    // Key at 1,2 should produce a X on long press on the first layer
    kaleidoscope::plugin::LongPressKey(0,
                                       KeyAddr(1, 2), Key_X),
    // Keys generating a D should produce a W on long press on the second layer
    kaleidoscope::plugin::LongPressKey(1,
                                       Key_D,         Key_W),
  )
}

void loop() {
  Kaleidoscope.loop();
}

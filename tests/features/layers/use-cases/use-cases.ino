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

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        Key_0, ___,   ___, ___, ___, ___, ___,
        Key_0, Key_0, ___, ___, ___, ___, ___,
        ShiftToLayer(0), ShiftToLayer(1), ShiftToLayer(2), ___, ___, ___,
        LockLayer(0),    LockLayer(1),    LockLayer(2),    ___, ___, ___, ___,
        MoveToLayer(0),  MoveToLayer(1),  MoveToLayer(2),  ___,
        ___,

        ___, Key_KeymapNext_Momentary, Key_KeymapPrevious_Momentary, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___, ___,
        ShiftToLayer(0), ShiftToLayer(1), ShiftToLayer(2), ___, ___, ___,
        ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___,
        ___
    ),
    [1] = KEYMAP_STACKED
    (
        ___,   Key_1, ___,   ___, ___, ___, ___,
        Key_1, ___,   Key_1, ___, ___, ___, ___,
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
    [2] = KEYMAP_STACKED
    (
        ___, ___,   Key_2, ___, ___, ___, ___,
        ___, Key_2, Key_2, ___, ___, ___, ___,
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

//KALEIDOSCOPE_INIT_PLUGINS();

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

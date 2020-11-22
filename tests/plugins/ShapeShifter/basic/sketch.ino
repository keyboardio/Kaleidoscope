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
#include <Kaleidoscope-ShapeShifter.h>

#include "./common.h"

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        Key_1, Key_2, Key_3, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___, ___,
        Key_LeftShift, ___, ___, ___, ___, ___,
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

static const kaleidoscope::plugin::ShapeShifter::dictionary_t shape_shifter_dictionary[] PROGMEM = {
  {Key_1, Key_2},
  {Key_2, Key_1},
  {Key_3, Key_4},
  {Key_NoKey, Key_NoKey},
};

KALEIDOSCOPE_INIT_PLUGINS(ShapeShifter);

void setup() {
  Kaleidoscope.setup();
  ShapeShifter.dictionary = shape_shifter_dictionary;
}

void loop() {
  Kaleidoscope.loop();
}

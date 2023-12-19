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
#include <Kaleidoscope-Cycle.h>

#include "./common.h"

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        Key_A, Key_B, Key_C, Key_D, Key_Cycle, ___, ___,
        ___,   ___,   ___,   ___,   ___, ___, ___,
        ___,   ___,   ___,   ___,   ___, ___,
        ___,   ___,   ___,   ___,   ___, ___, ___,
        ___,   ___,   ___,   ___,
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


void cycleAction(Key previous_key, uint8_t cycle_count) {
  if (previous_key == Key_A) {
    Cycle.replace(Key_1);
  } else if (previous_key == Key_B) {
    cycleThrough(Key_B, Key_1, Key_2);
  }
}

KALEIDOSCOPE_INIT_PLUGINS(Cycle);

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

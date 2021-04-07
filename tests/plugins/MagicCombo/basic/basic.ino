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
#include <Kaleidoscope-MagicCombo.h>

#include "./common.h"

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___, ___,
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

void tapKeyA(uint8_t magic_combo_index) {
  KeyAddr k{1, 0};
  Kaleidoscope.handleKeyEvent(KeyEvent{k, IS_PRESSED | INJECTED, Key_A});
  Kaleidoscope.handleKeyEvent(KeyEvent{k, WAS_PRESSED | INJECTED});
}

USE_MAGIC_COMBOS({.action = tapKeyA, .keys = {R0C0, R0C1, R0C2}});

KALEIDOSCOPE_INIT_PLUGINS(MagicCombo);

void setup() {
  Kaleidoscope.setup();
  MagicCombo.min_interval = 20;
}

void loop() {
  Kaleidoscope.loop();
}

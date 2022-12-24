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
#include <Kaleidoscope-Qukeys.h>


// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        ___, SFT_T(X), SFT_T(A), ___, ___, ___, ___,
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

KALEIDOSCOPE_INIT_PLUGINS(Qukeys);

void setup() {
  Kaleidoscope.setup();

  Qukeys.setHoldTimeout(10);
  Qukeys.setOverlapThreshold(0);
  Qukeys.setMinimumHoldTime(0);
  Qukeys.setMinimumPriorInterval(0);
  Qukeys.setMaxIntervalForTapRepeat(0);
}

void loop() {
  Kaleidoscope.loop();
}

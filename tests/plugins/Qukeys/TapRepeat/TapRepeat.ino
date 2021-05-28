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
#include <Kaleidoscope-Qukeys.h>

#include "./common.h"

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        Key_A, Key_LeftAlt, ___, ___, ___, ___, ___,
        SFT_T(J), ___, ___, ___, ___, ___, ___,
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

// Use Qukeys
KALEIDOSCOPE_INIT_PLUGINS(Qukeys);

void setup() {
  QUKEYS(
    kaleidoscope::plugin::Qukey(0, KeyAddr(0, 0), Key_LeftGui),
    kaleidoscope::plugin::Qukey(0, KeyAddr(0, 1), Key_B)
  )
  Qukeys.setHoldTimeout(kaleidoscope::testing::QUKEYS_HOLD_TIMEOUT);
  Qukeys.setOverlapThreshold(kaleidoscope::testing::QUKEYS_OVERLAP_THRESHOLD);
  Qukeys.setMinimumHoldTime(kaleidoscope::testing::QUKEYS_MINIMUM_HOLD_TIME);
  Qukeys.setMinimumPriorInterval(kaleidoscope::testing::QUKEYS_MIN_PRIOR_INTERVAL);
  Qukeys.setMaxIntervalForTapRepeat(kaleidoscope::testing::QUKEYS_MAX_TAP_REPEAT_INTERVAL);

  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

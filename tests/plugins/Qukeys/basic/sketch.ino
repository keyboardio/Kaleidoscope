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
#include <Kaleidoscope-Macros.h>

#include "./common.h"
#include "testing/default-layers.h"

using namespace kaleidoscope::testing::keymap;

KEYMAPS(LAYER_0);

KALEIDOSCOPE_INIT_PLUGINS(Qukeys);

void setup() {
  QUKEYS(
    kaleidoscope::plugin::Qukey(0, key_addr_A, Key_LeftGui),      // A/cmd
    kaleidoscope::plugin::Qukey(0, key_addr_F, Key_LeftShift),    // F/shift
  )
  Qukeys.setHoldTimeout(kaleidoscope::testing::QUKEYS_HOLD_TIMEOUT);
  Qukeys.setOverlapThreshold(kaleidoscope::testing::QUKEYS_OVERLAP_THRESHOLD);
  Qukeys.setMinimumHoldTime(kaleidoscope::testing::QUKEYS_MINIMUM_HOLD_TIME);

  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

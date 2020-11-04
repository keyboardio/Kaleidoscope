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
#include <Kaleidoscope-OneShot.h>

#include "./common.h"
#include "testing/default-layers.h"

using namespace kaleidoscope::testing::keymap;

KEYMAPS(LAYER_0, LAYER_1);

// Use Qukeys
KALEIDOSCOPE_INIT_PLUGINS(OneShot);

void setup() {
  Kaleidoscope.setup();

  OneShot.time_out = kaleidoscope::testing::ONESHOT_TIMEOUT;
  OneShot.hold_time_out = kaleidoscope::testing::ONESHOT_HOLD_TIMEOUT;
}

void loop() {
  Kaleidoscope.loop();
}

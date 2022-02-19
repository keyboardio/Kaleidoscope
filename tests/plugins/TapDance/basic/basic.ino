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
#include <Kaleidoscope-TapDance.h>

#include "./common.h"

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        Key_X, ___, ___, ___, ___, ___, ___,
        TD(0), ___, ___, ___, ___, ___, ___,
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

void tapDanceAction(uint8_t tap_dance_index,
                    KeyAddr key_addr,
                    uint8_t tap_count,
                    kaleidoscope::plugin::TapDance::ActionType tap_dance_action) {
  switch (tap_dance_index) {
  case 0:
    if (tap_dance_action == TapDance.Hold) {
      return tapDanceActionKeys(tap_count, tap_dance_action,
                                Key_A, Key_H, Key_C);
    } else {
      return tapDanceActionKeys(tap_count, tap_dance_action,
                                Key_A, Key_B, Key_C);
    }
  default:
    break;
  }
}

KALEIDOSCOPE_INIT_PLUGINS(TapDance);

void setup() {
  Kaleidoscope.setup();
  TapDance.time_out = 25;
}

void loop() {
  Kaleidoscope.loop();
}

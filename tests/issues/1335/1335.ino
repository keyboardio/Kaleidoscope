/* -*- mode: c++ -*-
 * Copyright (C) 2023  Keyboard.io, Inc.
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
#include <Kaleidoscope-Macros.h>
#include <Kaleidoscope-OneShot.h>

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        OSM(LeftShift), M(0), ___, ___, ___, ___, ___,
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

const macro_t *macroAction(uint8_t macro_id, KeyEvent &event) {
  if (keyToggledOn(event.state)) {
    switch (macro_id) {
    case 0:
      return MACRO(T(X), T(Y));
    }
  }
  return MACRO_NONE;
}

KALEIDOSCOPE_INIT_PLUGINS(OneShot, Macros);

void setup() {
  Kaleidoscope.setup();
  OneShot.setTimeout(50);
  OneShot.setHoldTimeout(20);
  OneShot.setDoubleTapTimeout(20);
}

void loop() {
  Kaleidoscope.loop();
}

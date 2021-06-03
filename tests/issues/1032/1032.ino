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

#include <Kaleidoscope-Syster.h>

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        SYSTER, ___, ___, ___, ___, ___, ___,
        Key_A, Key_B, Key_C, ___, ___, ___, ___,
        Key_0, Key_1, ___, ___, ___, ___,
        Key_Spacebar, ___, ___, ___, ___, ___, ___,
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

void systerAction(kaleidoscope::plugin::Syster::action_t action, const char *symbol) {
  switch (action) {
  case kaleidoscope::plugin::Syster::StartAction:
    break;
  case kaleidoscope::plugin::Syster::EndAction:
    break;
  case kaleidoscope::plugin::Syster::SymbolAction:
    if (strcmp(symbol, "abc") == 0) {
      Kaleidoscope.handleKeyEvent(KeyEvent(KeyAddr::none(), INJECTED | IS_PRESSED, Key_X));
      Kaleidoscope.handleKeyEvent(KeyEvent(KeyAddr::none(), INJECTED | WAS_PRESSED, Key_X));
    }
    if (strcmp(symbol, "a0") == 0) {
      Kaleidoscope.handleKeyEvent(KeyEvent(KeyAddr::none(), INJECTED | IS_PRESSED, Key_Y));
      Kaleidoscope.handleKeyEvent(KeyEvent(KeyAddr::none(), INJECTED | WAS_PRESSED, Key_Y));
    }
    break;
  }
}

KALEIDOSCOPE_INIT_PLUGINS(Syster);

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

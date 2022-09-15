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
#include <Kaleidoscope-SpaceCadet.h>

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        Key_LeftShift, Key_RightShift, ___, ___, ___, ___, ___,
        Key_A, Key_B, ___, ___, ___, ___, ___,
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

KALEIDOSCOPE_INIT_PLUGINS(SpaceCadet);

void setup() {
  Kaleidoscope.setup();

  //Set the SpaceCadet map
  //Setting is {KeyThatWasPressed, AlternativeKeyToSend, TimeoutInMS}
  //Note: must end with the SPACECADET_MAP_END delimiter
  static kaleidoscope::plugin::SpaceCadet::KeyBinding spacecadetmap[] = {
    {Key_LeftShift, Key_X, 10},
    {Key_RightShift, Key_Y, 0},
    {Key_LeftGui, Key_LeftCurlyBracket, 10},
    {Key_RightAlt, Key_RightCurlyBracket, 10},
    {Key_LeftAlt, Key_RightCurlyBracket, 10},
    {Key_LeftControl, Key_LeftBracket, 10},
    {Key_RightControl, Key_RightBracket, 10},
    SPACECADET_MAP_END
  };
  //Set the map.
  SpaceCadet.setMap(spacecadetmap);
  SpaceCadet.setTimeout(20);

  SpaceCadet.enableWithoutDelay();
}

void loop() {
  Kaleidoscope.loop();
}

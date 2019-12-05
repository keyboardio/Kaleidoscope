/* -*- mode: c++ -*-
 * Basic -- A very basic Kaleidoscope example
 * Copyright (C) 2018  Keyboard.io, Inc.
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

#include "Kaleidoscope.h"

#include "Kaleidoscope-LED-ActiveLayerColor.h"

// This example demonstrates the default key grouping mechanism
// where the left hand is assigned to key group 0 (KEY_GROUP_LEFT_HAND),
// the right hand to key group 1 (KEY_GROUP_RIGHT_HAND).

enum { AMap, BMap };

/* *INDENT-OFF* */
KEYMAPS(
  [AMap] = KEYMAP_STACKED
  (
   Key_A, Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,
   Key_A, Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,
   Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,
   Key_A, Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,

   Key_A, Key_A, Key_A, Key_A,
   KeyGroup(KEY_GROUP_RIGHT_HAND, ShiftToLayer(BMap)),

   Key_A, Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,
   Key_A, Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,
          Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,
   Key_A, Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,

   Key_A, Key_A, Key_A, Key_A,
   KeyGroup(KEY_GROUP_LEFT_HAND, ShiftToLayer(BMap))
  ),

  [BMap] = KEYMAP_STACKED
  (
   Key_B, Key_B, Key_B, Key_B, Key_B, Key_B, Key_B,
   Key_B, Key_B, Key_B, Key_B, Key_B, Key_B, Key_B,
   Key_B, Key_B, Key_B, Key_B, Key_B, Key_B,
   Key_B, Key_B, Key_B, Key_B, Key_B, Key_B, Key_B,

   Key_B, Key_B, Key_B, Key_B,
   ___,

   Key_B, Key_B, Key_B, Key_B, Key_B, Key_B, Key_B,
   Key_B, Key_B, Key_B, Key_B, Key_B, Key_B, Key_B,
          Key_B, Key_B, Key_B, Key_B, Key_B, Key_B,
   Key_B, Key_B, Key_B, Key_B, Key_B, Key_B, Key_B,

   Key_B, Key_B, Key_B, Key_B,
   ___
  )
)
/* *INDENT-ON* */

KALEIDOSCOPE_INIT_PLUGINS(
  LEDControl,
  LEDActiveLayerColorEffect
)

void setup() {
  Kaleidoscope.setup();

  static const cRGB layerColormap[] PROGMEM = {
    CRGB(128, 0, 0),
    CRGB(0, 128, 0)
  };

  LEDActiveLayerColorEffect.setColormap(layerColormap);
}

void loop() {
  Kaleidoscope.loop();
}

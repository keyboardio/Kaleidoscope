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

// This example demonstrates how keys can be grouped using conditionals
// in a decision function.
//
// Please note that the function groupOfKey(...) may only return values
// in the range [0;5].

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
   KeyGroup(KEY_GROUP_1, ShiftToLayer(BMap)),

   Key_A, Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,
   Key_A, Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,
          Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,
   Key_A, Key_A, Key_A, Key_A, Key_A, Key_A, Key_A,

   Key_A, Key_A, Key_A, Key_A,
   KeyGroup(KEY_GROUP_0 | KEY_GROUP_2, ShiftToLayer(BMap))
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

uint8_t groupOfKey(KeyAddr key_addr) {
  return key_addr.row();
}

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

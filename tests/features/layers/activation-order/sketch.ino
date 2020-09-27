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

#include "Kaleidoscope.h"

#include "Kaleidoscope-Macros.h"

// *INDENT-OFF*

KEYMAPS(
  [0] = KEYMAP_STACKED
  (
    Key_A ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A
   ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A
   ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A
   ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A
   ,Key_A ,Key_A ,Key_A ,M(0)
   ,ShiftToLayer(1)

   ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A
   ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A
          ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A
   ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A ,Key_A
   ,Key_A ,Key_A ,Key_A ,Key_A
   ,ShiftToLayer(2)
  ),

  [1] =  KEYMAP_STACKED
  (
    Key_B ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B
   ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B
   ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B
   ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B
   ,Key_B ,Key_B ,Key_B ,Key_B
   ,___

   ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B
   ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B
          ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B
   ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B ,Key_B
   ,Key_B ,Key_B ,Key_B ,Key_B
   ,___
  ),

  [2] =  KEYMAP_STACKED
  (
    Key_C ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C
   ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C
   ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C
   ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C
   ,Key_C ,Key_C ,Key_C ,Key_C
   ,___

   ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C
   ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C
          ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C
   ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C ,Key_C
   ,Key_C ,Key_C ,Key_C ,Key_C
   ,___
  )
) // KEYMAPS(

// *INDENT-ON*

KALEIDOSCOPE_INIT_PLUGINS(Macros);

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  switch (macroIndex) {
  case 0:
    if (keyToggledOn(keyState))
      Layer.deactivate(0);
    else
      Layer.activate(0);
    break;
  }
  return MACRO_NONE;
}

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

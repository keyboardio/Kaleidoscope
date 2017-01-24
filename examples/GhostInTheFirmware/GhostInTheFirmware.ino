/* -*- mode: c++ -*-
 * Akela -- Animated Keyboardio Extension Library for Anything
 * Copyright (C) 2017  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Akela-GhostInTheFirmware.h>
#include <Akela-LED-Stalker.h>
#include <Keyboardio-Macros.h>

const Key keymaps[][ROWS][COLS] PROGMEM = {
  [0] = KEYMAP_STACKED
  (
   Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, M(0),
   Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey,
   Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey,
   Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey,

   Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey,
   Key_NoKey,

   Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey,
   Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey,
              Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey,
   Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey,

   Key_NoKey, Key_NoKey, Key_NoKey, Key_NoKey,
   Key_NoKey
   ),
};

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  if (macroIndex == 0 && key_toggled_on (keyState))
    GhostInTheFirmware.activate ();

  return MACRO_NONE;
}

static const Akela::GhostInTheFirmware::GhostKey ghostKeys[] PROGMEM = {
  {0, 6, 5},
  {0, 5, 5},
  {0, 4, 5},
  {0, 3, 5},
  {0, 2, 5},
  {0, 1, 5},
  {0, 0, 5},
  {1, 0, 5},
  {1, 1, 5},
  {1, 2, 5},
  {1, 3, 5},
  {1, 4, 5},
  {1, 5, 5},
  {1, 6, 5},
  {2, 6, 5},
  {2, 5, 5},
  {2, 4, 5},
  {2, 3, 5},
  {2, 2, 5},
  {2, 1, 5},
  {2, 0, 5},
  {3, 0, 5},
  {3, 1, 5},
  {3, 3, 5},
  {3, 4, 5},
  {3, 5, 5},
  {0, 7, 5},
  {1, 7, 5},
  {2, 7, 5},
  {3, 7, 5},
  {3, 6, 5},

  {3, 9, 5},
  {3, 8, 5},
  {2, 8, 5},
  {1, 8, 5},
  {0, 8, 5},
  {3, 10, 5},
  {3, 11, 5},
  {3, 12, 5},
  {3, 13, 5},
  {3, 14, 5},
  {3, 15, 5},
  {2, 15, 5},
  {2, 14, 5},
  {2, 13, 5},
  {2, 12, 5},
  {2, 11, 5},
  {2, 10, 5},
  {2, 9, 5},
  {1, 9, 5},
  {1, 10, 5},
  {1, 11, 5},
  {1, 12, 5},
  {1, 13, 5},
  {1, 14, 5},
  {1, 15, 5},
  {0, 15, 5},
  {0, 14, 5},
  {0, 13, 5},
  {0, 12, 5},
  {0, 11, 5},
  {0, 10, 5},
  {0, 9, 5},

  {0, 0, 0}
};

void setup () {
  Keyboardio.setup (KEYMAP_SIZE);

  GhostInTheFirmware.configure (ghostKeys);
  StalkerEffect.configure (STALKER (BlazingTrail, NULL));

  Keyboardio.use (&GhostInTheFirmware, &StalkerEffect, &Macros, NULL);

}

void loop () {
  Keyboardio.loop ();
}

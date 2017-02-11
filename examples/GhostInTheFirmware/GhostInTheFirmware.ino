/* -*- mode: c++ -*-
 * Kaleidoscope-GhostInTheFirmware -- Let the keyboard write for you!
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

#include <Kaleidoscope.h>
#include <Kaleidoscope-GhostInTheFirmware.h>
#include <Kaleidoscope-LED-Stalker.h>
#include <Kaleidoscope-LEDControl.h>
#include <Kaleidoscope-Macros.h>

const Key keymaps[][ROWS][COLS] PROGMEM = {
  [0] = KEYMAP_STACKED
  (
   ___, ___, ___, ___, ___, ___, M(0),
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
};

static Key eventDropper (Key mappedKey, byte row, byte col, uint8_t keyState) {
  return Key_NoKey;
}

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  if (macroIndex == 0 && key_toggled_on (keyState))
    GhostInTheFirmware.activate ();

  return MACRO_NONE;
}

static const KaleidoscopePlugins::GhostInTheFirmware::GhostKey ghostKeys[] PROGMEM = {
  {0, 6, 200, 50},
  {0, 5, 200, 50},
  {0, 4, 200, 50},
  {0, 3, 200, 50},
  {0, 2, 200, 50},
  {0, 1, 200, 50},
  {0, 0, 200, 50},
  {1, 0, 200, 50},
  {1, 1, 200, 50},
  {1, 2, 200, 50},
  {1, 3, 200, 50},
  {1, 4, 200, 50},
  {1, 5, 200, 50},
  {1, 6, 200, 50},
  {2, 6, 200, 50},
  {2, 5, 200, 50},
  {2, 4, 200, 50},
  {2, 3, 200, 50},
  {2, 2, 200, 50},
  {2, 1, 200, 50},
  {2, 0, 200, 50},
  {3, 0, 200, 50},
  {3, 1, 200, 50},
  {3, 3, 200, 50},
  {3, 4, 200, 50},
  {3, 5, 200, 50},
  {0, 7, 200, 50},
  {1, 7, 200, 50},
  {2, 7, 200, 50},
  {3, 7, 200, 50},
  {3, 6, 200, 50},

  {3, 9, 200, 50},
  {3, 8, 200, 50},
  {2, 8, 200, 50},
  {1, 8, 200, 50},
  {0, 8, 200, 50},
  {3, 10, 200, 50},
  {3, 11, 200, 50},
  {3, 12, 200, 50},
  {3, 13, 200, 50},
  {3, 14, 200, 50},
  {3, 15, 200, 50},
  {2, 15, 200, 50},
  {2, 14, 200, 50},
  {2, 13, 200, 50},
  {2, 12, 200, 50},
  {2, 11, 200, 50},
  {2, 10, 200, 50},
  {2, 9, 200, 50},
  {1, 9, 200, 50},
  {1, 10, 200, 50},
  {1, 11, 200, 50},
  {1, 12, 200, 50},
  {1, 13, 200, 50},
  {1, 14, 200, 50},
  {1, 15, 200, 50},
  {0, 15, 200, 50},
  {0, 14, 200, 50},
  {0, 13, 200, 50},
  {0, 12, 200, 50},
  {0, 11, 200, 50},
  {0, 10, 200, 50},
  {0, 9, 200, 50},

  {0, 0, 0, 0}
};

void setup () {
  Serial.begin (9600);

  Kaleidoscope.setup (KEYMAP_SIZE);

  GhostInTheFirmware.configure (ghostKeys);
  StalkerEffect.configure (STALKER (BlazingTrail, NULL));

  Kaleidoscope.use (&LEDControl, &GhostInTheFirmware, &StalkerEffect, &Macros,
                    NULL);
  event_handler_hook_use (eventDropper);
}

void loop () {
  Kaleidoscope.loop ();
}

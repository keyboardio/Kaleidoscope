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
#include <Kaleidoscope-Leader.h>
#include <Kaleidoscope-Macros.h>

// *INDENT-OFF*
KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        LEAD(0), ___, ___, ___, ___, ___, ___,
        Key_A, Key_B, Key_C, Key_D, Key_C, ___, ___,
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

KALEIDOSCOPE_INIT_PLUGINS(Leader);

static void leaderAB(uint8_t id) {
  Macros.type(PSTR("z"));
}

static void leaderAC(uint8_t id) {
  Macros.type(PSTR("xyz"));
}

static void leaderBA(uint8_t id) {
  Macros.type(PSTR("y"));
}

static void leaderBC(uint8_t id) {
  Macros.type(PSTR("x"));
}

static void leaderC(uint8_t id) {
  Macros.type(PSTR("q"));
}

// *INDENT-OFF*
static const kaleidoscope::plugin::Leader::dictionary_t leader_dictionary[] PROGMEM =
  LEADER_DICT( {LEADER_SEQ(LEAD(0), Key_A, Key_B), leaderAB},
               {LEADER_SEQ(LEAD(0), Key_A, Key_C), leaderAC},
               {LEADER_SEQ(LEAD(0), Key_B, Key_A), leaderBA},
               {LEADER_SEQ(LEAD(0), Key_B, Key_C), leaderBC},
               {LEADER_SEQ(LEAD(0), Key_C),        leaderC }  );
// *INDENT-ON*

void setup() {
  Kaleidoscope.setup();
  Leader.setTimeout(20);
  Leader.dictionary = leader_dictionary;
}

void loop() {
  Kaleidoscope.loop();
}

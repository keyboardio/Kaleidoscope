/* -*- mode: c++ -*-
 * Splitography -- A very basic Kaleidoscope example for the SOFT/HRUF Splitography keyboard
 * Copyright (C) 2018  Keyboard.io, Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTabILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "Kaleidoscope.h"
#include "Kaleidoscope-Steno.h"

enum {
  _STENO,
};

/* *INDENT-OFF* */
KEYMAPS(

/* Steno (GeminiPR)
 * ,-----------------------.      ,-----------------------.
 * | # | # | # | # | # | # |      | # | # | # | # | # | # |
 * |---+---+---+---+---+---|      |---+---+---+---+---+---|
 * |   | S | T | P | H | * |      | * | F | P | L | T | D |
 * |---+---+---+---+---+---|      |---+---+---+---+---+---|
 * |   | S | K | W | R | * |      | * | R | B | G | S | Z |
 * `-------------+---+---+-'      `-+---+---+-------------'
 *               | A | O |          | E | U |
 *               `-------'          `-------'
 */
 [_STENO] = KEYMAP(
    S(N1) ,S(N2) ,S(N3) ,S(N4) ,S(N5) ,S(N6)    ,S(N7)  ,S(N8) ,S(N9) ,S(NA) ,S(NB) ,S(NC)
   ,XXX   ,S(S1) ,S(TL) ,S(PL) ,S(HL) ,S(ST1)   ,S(ST3) ,S(FR) ,S(PR) ,S(LR) ,S(TR) ,S(DR)
   ,XXX   ,S(S2) ,S(KL) ,S(WL) ,S(RL) ,S(ST2)   ,S(ST4) ,S(RR) ,S(BR) ,S(GR) ,S(SR) ,S(ZR)
                               ,S(A)  ,S(O)     ,S(E)   ,S(U)
  )
);
/* *INDENT-ON* */

KALEIDOSCOPE_INIT_PLUGINS(GeminiPR);

void setup() {
  Kaleidoscope.serialPort().begin(9600);
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}

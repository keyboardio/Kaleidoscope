/* Kaleidoscope-LED-AlphaSquare -- 4x4 pixel LED alphabet
 * Copyright (C) 2017  Keyboard.io, Inc
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

// clang-format off

#pragma once

#ifndef KALEIDOSCOPE_LED_FONT
#define KALEIDOSCOPE_LED_FONT

#define ALPHASQUARE_SYMBOL_A  SYM4x4(1, 1, 1, 1, \
                                     1, 0, 0, 1, \
                                     1, 1, 1, 1, \
                                     1, 0, 0, 1)
#define ALPHASQUARE_SYMBOL_B  SYM4x4(1, 1, 1, 1, \
                                     1, 0, 1, 1, \
                                     1, 1, 0, 1, \
                                     1, 1, 1, 1)
#define ALPHASQUARE_SYMBOL_C  SYM4x4(1, 1, 1, 1, \
                                     1, 0, 0, 0, \
                                     1, 0, 0, 0, \
                                     1, 1, 1, 1)
#define ALPHASQUARE_SYMBOL_D  SYM4x4(1, 1, 1, 0, \
                                     1, 0, 0, 1, \
                                     1, 0, 0, 1, \
                                     1, 1, 1, 0)
#define ALPHASQUARE_SYMBOL_E  SYM4x4(1, 1, 1, 1, \
                                     1, 1, 0, 0, \
                                     1, 0, 0, 0, \
                                     1, 1, 1, 1)
#define ALPHASQUARE_SYMBOL_F  SYM4x4(1, 1, 1, 1, \
                                     1, 0, 0, 0, \
                                     1, 1, 1, 0, \
                                     1, 0, 0, 0)
#define ALPHASQUARE_SYMBOL_G  SYM4x4(1, 1, 1, 0, \
                                     1, 0, 0, 0, \
                                     1, 0, 0, 1, \
                                     1, 1, 1, 1)
#define ALPHASQUARE_SYMBOL_H  SYM4x4(1, 0, 0, 1, \
                                     1, 1, 1, 1, \
                                     1, 0, 0, 1, \
                                     1, 0, 0, 1)
#define ALPHASQUARE_SYMBOL_I  SYM4x4(1, 1, 1, 1, \
                                     0, 1, 1, 0, \
                                     0, 1, 1, 0, \
                                     1, 1, 1, 1)
#define ALPHASQUARE_SYMBOL_J  SYM4x4(1, 1, 1, 1, \
                                     0, 0, 0, 1, \
                                     1, 0, 0, 1, \
                                     0, 1, 1, 0)
#define ALPHASQUARE_SYMBOL_K  SYM4x4(1, 0, 0, 1, \
                                     1, 1, 0, 0, \
                                     1, 1, 0, 0, \
                                     1, 0, 1, 1)
#define ALPHASQUARE_SYMBOL_L  SYM4x4(1, 0, 0, 0, \
                                     1, 0, 0, 0, \
                                     1, 0, 0, 0, \
                                     1, 1, 1, 1)
#define ALPHASQUARE_SYMBOL_M  SYM4x4(1, 0, 1, 1, \
                                     1, 1, 1, 1, \
                                     1, 1, 0, 1, \
                                     1, 0, 0, 1)
#define ALPHASQUARE_SYMBOL_N  SYM4x4(1, 0, 0, 1, \
                                     1, 1, 0, 1, \
                                     1, 0, 1, 1, \
                                     1, 0, 0, 1)
#define ALPHASQUARE_SYMBOL_O  SYM4x4(1, 1, 1, 1, \
                                     1, 0, 0, 1, \
                                     1, 0, 0, 1, \
                                     1, 1, 1, 1)
#define ALPHASQUARE_SYMBOL_P  SYM4x4(1, 1, 1, 1, \
                                     1, 0, 0, 1, \
                                     1, 1, 1, 1, \
                                     1, 0, 0, 0)
#define ALPHASQUARE_SYMBOL_Q  SYM4x4(1, 1, 1, 1, \
                                     1, 0, 0, 1, \
                                     1, 0, 1, 1, \
                                     1, 1, 1, 1)
#define ALPHASQUARE_SYMBOL_R  SYM4x4(1, 1, 1, 1, \
                                     1, 0, 0, 1, \
                                     1, 1, 1, 0, \
                                     1, 0, 1, 1)
#define ALPHASQUARE_SYMBOL_S  SYM4x4(1, 1, 1, 1, \
                                     1, 1, 0, 0, \
                                     0, 0, 1, 1, \
                                     1, 1, 1, 1)
#define ALPHASQUARE_SYMBOL_T  SYM4x4(1, 1, 1, 1, \
                                     0, 1, 1, 0, \
                                     0, 1, 1, 0, \
                                     0, 1, 1, 0)
#define ALPHASQUARE_SYMBOL_U  SYM4x4(1, 0, 0, 1, \
                                     1, 0, 0, 1, \
                                     1, 0, 0, 1, \
                                     1, 1, 1, 1)
#define ALPHASQUARE_SYMBOL_V  SYM4x4(1, 0, 0, 1, \
                                     1, 0, 0, 1, \
                                     1, 0, 0, 1, \
                                     0, 1, 1, 0)
#define ALPHASQUARE_SYMBOL_W  SYM4x4(1, 0, 0, 1, \
                                     1, 0, 1, 1, \
                                     1, 1, 1, 1, \
                                     1, 0, 1, 1)
#define ALPHASQUARE_SYMBOL_X  SYM4x4(1, 0, 0, 1, \
                                     0, 1, 1, 0, \
                                     0, 1, 1, 0, \
                                     1, 0, 0, 1)
#define ALPHASQUARE_SYMBOL_Y  SYM4x4(1, 0, 0, 1, \
                                     1, 1, 1, 1, \
                                     0, 1, 1, 0, \
                                     0, 1, 1, 0)
#define ALPHASQUARE_SYMBOL_Z  SYM4x4(1, 1, 1, 1, \
                                     0, 0, 1, 1, \
                                     1, 1, 0, 0, \
                                     1, 1, 1, 1)
// ---------------------
#define ALPHASQUARE_SYMBOL_1  SYM4x4(0, 1, 1, 0, \
                                     1, 0, 1, 0, \
                                     0, 0, 1, 0, \
                                     1, 1, 1, 1)
#define ALPHASQUARE_SYMBOL_2  SYM4x4(0, 1, 1, 0, \
                                     1, 0, 0, 1, \
                                     0, 0, 1, 0, \
                                     1, 1, 0, 1)
#define ALPHASQUARE_SYMBOL_3  SYM4x4(1, 1, 1, 1, \
                                     0, 0, 1, 1, \
                                     0, 0, 0, 1, \
                                     1, 1, 1, 1)
#define ALPHASQUARE_SYMBOL_4  SYM4x4(1, 0, 0, 1, \
                                     1, 1, 1, 1, \
                                     0, 0, 0, 1, \
                                     0, 0, 0, 1)
#define ALPHASQUARE_SYMBOL_5  SYM4x4(1, 1, 1, 1, \
                                     1, 0, 0, 0, \
                                     0, 1, 1, 1, \
                                     1, 1, 1, 1)
#define ALPHASQUARE_SYMBOL_6  SYM4x4(0, 1, 1, 0, \
                                     1, 0, 0, 0, \
                                     1, 1, 1, 1, \
                                     1, 1, 1, 1)
#define ALPHASQUARE_SYMBOL_7  SYM4x4(1, 1, 1, 1, \
                                     0, 0, 0, 1, \
                                     0, 0, 1, 0, \
                                     0, 1, 0, 0)
#define ALPHASQUARE_SYMBOL_8  SYM4x4(1, 1, 1, 0, \
                                     1, 0, 1, 1, \
                                     1, 1, 0, 1, \
                                     0, 1, 1, 1)
#define ALPHASQUARE_SYMBOL_9  SYM4x4(1, 1, 1, 1, \
                                     1, 0, 0, 1, \
                                     1, 1, 1, 1, \
                                     0, 0, 0, 1)
#define ALPHASQUARE_SYMBOL_0  SYM4x4(0, 1, 1, 0, \
                                     1, 0, 0, 1, \
                                     1, 0, 0, 1, \
                                     0, 1, 1, 0)

#endif

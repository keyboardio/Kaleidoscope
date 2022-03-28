/*
 * Based on Light_WS2812, from:
 *  https://github.com/cpldcpu/light_ws2812
 *
 * Original copyright:
 *
 *  light weight WS2812 lib V2.1 - Arduino support
 *
 *  Controls WS2811/WS2812/WS2812B RGB-LEDs
 *  Author: Matthias Riegler
 *
 *  Mar 07 2014: Added Arduino and C++ Library
 *
 *   September 6, 2014:    Added option to switch between most popular color orders
 *                                               (RGB, GRB, and BRG) --  Windell H. Oskay
 *
 * License:
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

#pragma once

// Timing in ns
#define w_zeropulse   350
#define w_onepulse    900
#define w_totalperiod 1250

// Fixed cycles used by the inner loop
#define w_fixedlow   2
#define w_fixedhigh  4
#define w_fixedtotal 8

// Insert NOPs to match the timing, if possible
#define w_zerocycles  (((F_CPU / 1000) * w_zeropulse) / 1000000)
#define w_onecycles   (((F_CPU / 1000) * w_onepulse + 500000) / 1000000)
#define w_totalcycles (((F_CPU / 1000) * w_totalperiod + 500000) / 1000000)

// w1 - nops between rising edge and falling edge - low
#define w1 (w_zerocycles - w_fixedlow)
// w2   nops between fe low and fe high
#define w2 (w_onecycles - w_fixedhigh - w1)
// w3   nops to complete loop
#define w3 (w_totalcycles - w_fixedtotal - w1 - w2)

#if w1 > 0
#define w1_nops w1
#else
#define w1_nops 0
#endif

// The only critical timing parameter is the minimum pulse length of the "0"
// Warn or throw error if this timing can not be met with current F_CPU settings.
#define w_lowtime ((w1_nops + w_fixedlow) * 1000000) / (F_CPU / 1000)
#if w_lowtime > 550
#error "Light_ws2812: Sorry, the clock speed is too low. Did you set F_CPU correctly?"
#elif w_lowtime > 450
#warning "Light_ws2812: The timing is critical and may only work on WS2812B, not on WS2812(S)."
#warning "Please consider a higher clockspeed, if possible"
#endif

#if w2 > 0
#define w2_nops w2
#else
#define w2_nops 0
#endif

#if w3 > 0
#define w3_nops w3
#else
#define w3_nops 0
#endif

#define w_nop1  "nop      \n\t"
#define w_nop2  "rjmp .+0 \n\t"
#define w_nop4  w_nop2 w_nop2
#define w_nop8  w_nop4 w_nop4
#define w_nop16 w_nop8 w_nop8

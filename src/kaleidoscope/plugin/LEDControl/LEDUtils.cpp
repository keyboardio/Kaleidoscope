/* Kaleidoscope-LEDControl - LED control plugin for Kaleidoscope
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
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

#include "kaleidoscope/plugin/LEDControl/LEDUtils.h"

#include "kaleidoscope/Runtime.h"  // for Runtime, Runtime_

cRGB breath_compute(uint8_t hue, uint8_t saturation, uint8_t phase_offset) {

  using kaleidoscope::Runtime;

  // This code is adapted from FastLED lib8tion.h as of dd5d96c6b289cb6b4b891748a4aeef3ddceaf0e6
  // Eventually, we should consider just using FastLED

  // The phase offset is provided in case one wants more than one breathe effect
  // differing in phase at the same time. This may be useful for individual
  // indicators that need to contrast with any other overall breathe effect.
  // (Offset value 128 giving 50% ie opposite phase is especially nice.)
  //
  // The actual breathe computation function has a period of 4096. The input
  // phase offset ranges from 0 to 255. This is to be scaled to the actual
  // period 0 to 4096. To allow precise offsets of 25%, 50% etc, and for
  // computational convenience and speed (and since one would not meaningfully
  // request an offset of 4096 for 255 to be mapped to it), we merely multiply
  // the input offset by 16 (== 4096/256), or equivalently lshift it by 4.

  // We do a bit shift here instead of division to ensure that there's no discontinuity
  // in the output brightness when the integer overflows.
  uint8_t i = ((uint16_t)Runtime.millisAtCycleStart() + (phase_offset << 4)) >> 4;

  if (i & 0x80) {
    i = 255 - i;
  }

  i           = i << 1;
  uint8_t ii  = (i * i) >> 8;
  uint8_t iii = (ii * i) >> 8;

  i = (((3 * (uint16_t)(ii)) - (2 * (uint16_t)(iii))) / 2) + 80;

  return hsvToRgb(hue, saturation, i);
}

//For rgb to hsv, might take a look at:  http://web.mit.edu/storborg/Public/hsvtorgb.c


// From http://web.mit.edu/storborg/Public/hsvtorgb.c - talk to Scott about licensing
cRGB hsvToRgb(uint16_t h, uint16_t s, uint16_t v) {
  cRGB color;

  /* HSV to RGB conversion function with only integer
   * math */
  uint16_t region, fpart, p, q, t;

  if (s == 0) {
    /* color is grayscale */
    color.r = color.g = color.b = v;
    return color;
  }

  /* make hue 0-5 */
  region = (h * 6) >> 8;
  /* find remainder part, make it from 0-255 */
  fpart = (h * 6) - (region << 8);

  /* calculate temp vars, doing integer multiplication */
  p = (v * (255 - s)) >> 8;
  q = (v * (255 - ((s * fpart) >> 8))) >> 8;
  t = (v * (255 - ((s * (255 - fpart)) >> 8))) >> 8;

  /* assign temp vars based on color cone region */
  switch (region) {
  case 0:
    color.r = v;
    color.g = t;
    color.b = p;
    break;
  case 1:
    color.r = q;
    color.g = v;
    color.b = p;
    break;
  case 2:
    color.r = p;
    color.g = v;
    color.b = t;
    break;
  case 3:
    color.r = p;
    color.g = q;
    color.b = v;
    break;
  case 4:
    color.r = t;
    color.g = p;
    color.b = v;
    break;
  default:
    color.r = v;
    color.g = p;
    color.b = q;
    break;
  }

  return color;
}

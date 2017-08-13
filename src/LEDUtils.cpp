#include "LEDUtils.h"

cRGB
breath_compute() {
  // This code is adapted from FastLED lib8tion.h as of dd5d96c6b289cb6b4b891748a4aeef3ddceaf0e6
  // Eventually, we should consider just using FastLED

  uint8_t i = (uint16_t)millis() / 12;

  if (i & 0x80) {
    i = 255 - i;
  }

  i = i << 1;
  uint8_t ii = (i * i) >> 8;
  uint8_t iii = (ii * i) >> 8;

  i = (((3 * (uint16_t)(ii)) - (2 * (uint16_t)(iii))) / 2) + 80;

  return hsvToRgb(170, 255, i);
}



// From http://web.mit.edu/storborg/Public/hsvtorgb.c - talk to Scott about licensing
cRGB
hsvToRgb(uint16_t h, uint16_t s, uint16_t v) {
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

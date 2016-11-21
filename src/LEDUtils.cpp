#include "LEDUtils.h"

cRGB
breath_compute (BreathState *state) {
  // algorithm from http://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
  state->brightness =  (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;
  // change the brightness for next time through the loop:
  //state->brightness = state->brightness + state->fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (state->brightness == 0 || state->brightness == 150) {
    state->fadeAmount = -state->fadeAmount ;
  }

  return hsv_to_rgb(200, 255, state->brightness);
}

// From http://web.mit.edu/storborg/Public/hsvtorgb.c - talk to Scott about licensing
cRGB
hsv_to_rgb(uint16_t h, uint16_t s, uint16_t v) {
  cRGB color;

  /* HSV to RGB conversion function with only integer
   * math */
  uint16_t region, fpart, p, q, t;

  if(s == 0) {
    /* color is grayscale */
    color.r = color.g = color.b = v;
    return color;
  }

  /* make hue 0-5 */
  region = (h *6) >> 8;
  /* find remainder part, make it from 0-255 */
  fpart = (h*6) - (region <<8);

  /* calculate temp vars, doing integer multiplication */
  p = (v * (255 - s)) >> 8;
  q = (v * (255 - ((s * fpart) >> 8))) >> 8;
  t = (v * (255 - ((s * (255 - fpart)) >> 8))) >> 8;

  /* assign temp vars based on color cone region */
  switch(region) {
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

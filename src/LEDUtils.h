#pragma once

#include "KeyboardConfig.h"

typedef struct BreathState {
  uint8_t brightness = 0;    // how bright the LED is
  int8_t fadeAmount  = 1;    // how many points to fade the LED by (can be negative)
};

cRGB breath_compute (BreathState *state);
cRGB hsv_to_rgb(uint16_t h, uint16_t s, uint16_t v);

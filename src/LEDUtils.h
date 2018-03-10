#pragma once

#include <Kaleidoscope.h>

cRGB breath_compute(void);
cRGB breath_compute(uint8_t hue);
cRGB breath_compute(uint8_t hue, uint8_t saturation);
cRGB hsvToRgb(uint16_t h, uint16_t s, uint16_t v);

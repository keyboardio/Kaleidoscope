#include "Kaleidoscope-LEDEffect-SolidColor.h"

namespace kaleidoscope {
LEDSolidColor::LEDSolidColor(uint8_t r, uint8_t g, uint8_t b) {
  this->r = r;
  this->g = g;
  this->b = b;
}

void LEDSolidColor::onActivate(void) {
  ::LEDControl.set_all_leds_to(r, g, b);
}

void LEDSolidColor::refreshAt(byte row, byte col) {
  ::LEDControl.setCrgbAt(row, col, CRGB(r, g, b));
}
}

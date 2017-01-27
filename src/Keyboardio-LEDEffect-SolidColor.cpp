#include "Keyboardio-LEDEffect-SolidColor.h"

LEDSolidColor::LEDSolidColor (uint8_t r, uint8_t g, uint8_t b) {
  this->r = r;
  this->g = g;
  this->b = b;
  LEDControl.mode_add (this);
}

void
LEDSolidColor::init (void) {
  LEDControl.set_all_leds_to (r, g, b);
}

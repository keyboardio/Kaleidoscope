#include "Kaleidoscope-LEDEffect-Breathe.h"

LEDBreatheEffect_::LEDBreatheEffect_(void) {
}

void LEDBreatheEffect_::update(void) {
  cRGB color = breath_compute();
  LEDControl.set_all_leds_to(color);
}

LEDBreatheEffect_ LEDBreatheEffect;

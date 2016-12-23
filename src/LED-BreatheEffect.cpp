#include "LED-BreatheEffect.h"

LEDBreatheEffect_::LEDBreatheEffect_ (void) {
  state.brightness = 0;
  state.fadeAmount = 1;
}

void
LEDBreatheEffect_::update (void) {
  cRGB color = breath_compute (&state);
  LEDControl.set_all_leds_to (color);
}

LEDBreatheEffect_ LEDBreatheEffect;

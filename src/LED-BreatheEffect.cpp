#include "LED-BreatheEffect.h"

LEDBreatheEffect::LEDBreatheEffect (void) {
  state.brightness = 0;
  state.fadeAmount = 1;

  LEDControl.mode_add (this);
}

void
LEDBreatheEffect::update (void) {
  cRGB color = breath_compute (&state);
  LEDControl.set_all_leds_to (color);
}

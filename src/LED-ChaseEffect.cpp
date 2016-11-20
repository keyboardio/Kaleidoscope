#include "LED-ChaseEffect.h"

LEDChaseEffect::LEDChaseEffect (void) {
  LEDControl.mode_add (this);
}

void
LEDChaseEffect::update (void) {
  if (current_chase_counter++ < chase_threshold) {
    return;
  }
  current_chase_counter = 0;
  led_set_crgb_at(pos - (chase_sign* chase_pixels), {0, 0, 0});
  led_set_crgb_at(pos, {0, 0, 0});

  pos += chase_sign;
  if (pos >= LED_COUNT || pos <= 0) {
    chase_sign = -chase_sign;
  }
  led_set_crgb_at(pos, {0, 0, 255});
  led_set_crgb_at(pos - (chase_sign * chase_pixels), {255, 0, 0});
}

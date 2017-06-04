#include "Kaleidoscope-LEDEffect-Chase.h"

LEDChaseEffect_::LEDChaseEffect_(void) {
}

void
LEDChaseEffect_::update(void) {
  if (current_chase_counter++ < chase_threshold) {
    return;
  }
  current_chase_counter = 0;
  LEDControl.led_set_crgb_at(pos - (chase_sign* chase_pixels), {0, 0, 0});
  LEDControl.led_set_crgb_at(pos, {0, 0, 0});

  pos += chase_sign;
  if (pos >= LED_COUNT || pos <= 0) {
    chase_sign = -chase_sign;
  }
  LEDControl.led_set_crgb_at(pos, {0, 0, 255});
  LEDControl.led_set_crgb_at(pos - (chase_sign * chase_pixels), {255, 0, 0});
}

LEDChaseEffect_ LEDChaseEffect;

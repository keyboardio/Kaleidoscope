#include "Kaleidoscope-LEDEffect-Breathe.h"
#define UPDATE_INTERVAL 50  // milliseconds between two LED updates to avoid overloading; 20 fps

namespace kaleidoscope {
void LEDBreatheEffect::update(void) {
  uint16_t now = millis();
  if ((now - last_update) < UPDATE_INTERVAL)
    return;
  last_update = now;

  cRGB color = breath_compute(hue, saturation);
  ::LEDControl.set_all_leds_to(color);
}
}

kaleidoscope::LEDBreatheEffect LEDBreatheEffect;

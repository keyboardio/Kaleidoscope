#include "Kaleidoscope-LEDEffect-Chase.h"

namespace kaleidoscope {
void LEDChaseEffect::update(void) {
  if (current_chase_counter++ < chase_threshold) {
    return;
  }
  current_chase_counter = 0;

  byte pos2 = pos - (chase_sign * chase_pixels);

  ::LEDControl.setCrgbAt(pos, {0, 0, 0});
  if (pos2 < LED_COUNT)
    ::LEDControl.setCrgbAt(pos2, {0, 0, 0});

  pos += chase_sign;
  if (! (pos < LED_COUNT)) {
    chase_sign = -chase_sign;
    pos += chase_sign;
    pos2 = LED_COUNT;
  } else {
    pos2 += chase_sign;
  }

  ::LEDControl.setCrgbAt(pos, {0, 0, 255});
  if (pos2 < LED_COUNT)
    ::LEDControl.setCrgbAt(pos2, {255, 0, 0});
}
}

kaleidoscope::LEDChaseEffect LEDChaseEffect;

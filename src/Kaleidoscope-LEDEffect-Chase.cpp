#include "Kaleidoscope-LEDEffect-Chase.h"

namespace kaleidoscope {
void LEDChaseEffect::update(void) {
  // Check to see if it's time to change the positions of the red and blue lights
  if (current_chase_counter++ < chase_threshold) {
    return;
  }
  current_chase_counter = 0;

  // The red LED is at `pos`; the blue one follows behind. `chase_sign` is either +1 or
  // -1; `chase_pixels` is the gap between them.
  byte pos2 = pos - (chase_sign * chase_pixels);

  // First, we turn off the LEDs that were turned on in the previous update. `pos` is
  // always in the valid range (0 <= pos < LED_COUNT), but after it changes direction, for
  // the first few updates, `pos2` will be out of bounds. Since it's an unsigned integer,
  // even when it would have a value below zero, it underflows and so one test is good for
  // both ends of the range.
  ::LEDControl.setCrgbAt(pos, {0, 0, 0});
  if (pos2 < LED_COUNT)
    ::LEDControl.setCrgbAt(pos2, {0, 0, 0});

  // Next, we adjust the red light's position. If the direction hasn't changed (the red
  // light isn't out of bounds), we also adjust the blue light's position to match the red
  // one. If the new position puts it out of bounds, we reverse the direction, and bring
  // it back in bounds. When this happens, the blue light "jumps" behind the red one, and
  // will be out of bounds. The simplest way to do this is to assign it a value that is
  // known to be invalid (LED_COUNT).
  pos += chase_sign;
  if (pos < LED_COUNT) {
    pos2 += chase_sign;
  } else {
    chase_sign = -chase_sign;
    pos += chase_sign;
    pos2 = LED_COUNT;
  }

  // Last, we turn on the LEDs at their new positions. As before, the blue light (pos2) is
  // only set if it's in the valid LED range.
  ::LEDControl.setCrgbAt(pos, {0, 0, 255});
  if (pos2 < LED_COUNT)
    ::LEDControl.setCrgbAt(pos2, {255, 0, 0});
}
}

kaleidoscope::LEDChaseEffect LEDChaseEffect;

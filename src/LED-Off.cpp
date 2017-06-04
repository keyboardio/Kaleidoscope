#include "LED-Off.h"

void LEDOff_::update(void) {
  LEDControl.set_all_leds_to({0, 0, 0});
}

LEDOff_ LEDOff;

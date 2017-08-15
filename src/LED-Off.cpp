#include "LED-Off.h"

namespace kaleidoscope {
void LEDOff::onActivate(void) {
  ::LEDControl.set_all_leds_to({0, 0, 0});
}

void LEDOff::refreshAt(byte row, byte col) {
  ::LEDControl.setCrgbAt(row, col, {0, 0, 0});
}
}

kaleidoscope::LEDOff LEDOff;

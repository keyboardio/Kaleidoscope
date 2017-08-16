#pragma once

#include "Kaleidoscope-LEDControl.h"

namespace kaleidoscope {
class LEDOff : public LEDMode {
 public:
  LEDOff(void) { }

 protected:
  void onActivate(void) final;
  void refreshAt(byte row, byte col) final;
};
}

extern kaleidoscope::LEDOff LEDOff;

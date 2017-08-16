#pragma once

#include "Kaleidoscope-LEDControl.h"

namespace kaleidoscope {
class LEDSolidColor : public LEDMode {
 public:
  LEDSolidColor(uint8_t r, uint8_t g, uint8_t b);

 protected:
  void onActivate(void) final;
  void refreshAt(byte row, byte col) final;

 private:
  uint8_t r, g, b;
};
}

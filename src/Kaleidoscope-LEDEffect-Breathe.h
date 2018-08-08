#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "LEDUtils.h"

namespace kaleidoscope {
class LEDBreatheEffect : public LEDMode {
 public:
  LEDBreatheEffect(void) {}

  uint8_t hue = 170;
  uint8_t saturation = 255;

 protected:
  void update(void) final;

 private:
  uint16_t last_update = 0;
};
}

extern kaleidoscope::LEDBreatheEffect LEDBreatheEffect;

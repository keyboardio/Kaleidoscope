#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "LEDUtils.h"

namespace kaleidoscope {
class LEDBreatheEffect : public LEDMode {
 public:
  LEDBreatheEffect(void) {}

 protected:
  void update(void) final;
};
}

extern kaleidoscope::LEDBreatheEffect LEDBreatheEffect;

#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "LEDUtils.h"

class LEDBreatheEffect_ : public LEDMode {
 public:
  LEDBreatheEffect_(void);

  void update(void) final;
};

extern LEDBreatheEffect_ LEDBreatheEffect;

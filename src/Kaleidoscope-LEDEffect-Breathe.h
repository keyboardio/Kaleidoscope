#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "LEDUtils.h"

class LEDBreatheEffect_ : LEDMode {
 public:
  LEDBreatheEffect_ (void);

  virtual void update (void) final;
};

extern LEDBreatheEffect_ LEDBreatheEffect;

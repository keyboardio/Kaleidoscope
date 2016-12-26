#pragma once

#include "LEDControl.h"
#include "LEDUtils.h"

class LEDBreatheEffect_ : LEDMode {
 public:
  LEDBreatheEffect_ (void);

  virtual void update (void) final;

 private:
  BreathState state;
};

extern LEDBreatheEffect_ LEDBreatheEffect;

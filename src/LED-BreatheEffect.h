#pragma once

#include "LEDControl.h"
#include "LEDUtils.h"

class LEDBreatheEffect : LEDMode {
 public:
  LEDBreatheEffect (void);

  virtual void update (void) final;

 private:
  BreathState state;
};

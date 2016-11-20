#pragma once

#include "LEDControl.h"

class LEDBootAnimation : LEDMode {
 public:
  LEDBootAnimation (void);

  virtual void setup (void) final;
};

#pragma once

#include "LEDControl.h"
#include "LEDUtils.h"

class LEDNumlock : LEDMode {
 public:
  LEDNumlock (uint8_t numpadIndex);

  virtual void update (void) final;
  virtual void setup (void) final;

 private:
  static void loopHook (void);

  BreathState breathState;
};

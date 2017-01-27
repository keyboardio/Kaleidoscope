#pragma once

#include "Keyboardio-LEDControl.h"
#include "LEDUtils.h"

class LEDNumlock : LEDMode {
 public:
  LEDNumlock (uint8_t numpadIndex);

  virtual void begin (void) final;

  virtual void update (void) final;
  virtual void init (void) final;

 private:
  static void loopHook (bool postClear);
};

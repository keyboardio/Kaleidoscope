#pragma once

#include "LEDControl.h"

class LEDSolidColor : LEDMode {
 public:
  LEDSolidColor (uint8_t r, uint8_t g, uint8_t b);

  virtual void init (void) final;

 private:
  uint8_t r, g, b;
};

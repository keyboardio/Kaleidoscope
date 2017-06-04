#pragma once

#include "Kaleidoscope-LEDControl.h"

class LEDOff_ : public LEDMode {
 public:
  LEDOff_(void) { };

  virtual void update(void) final;
};

extern LEDOff_ LEDOff;

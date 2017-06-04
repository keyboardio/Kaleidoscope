#pragma once

#include "Kaleidoscope-LEDControl.h"

class LEDOff_ : public LEDMode {
 public:
  LEDOff_(void) { }

  void update(void) final;
};

extern LEDOff_ LEDOff;

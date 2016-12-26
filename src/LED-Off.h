#pragma once

#include "LEDControl.h"

class LEDOff_ : public LEDMode {
 public:
  LEDOff_ (void) { };
};

extern LEDOff_ LEDOff;

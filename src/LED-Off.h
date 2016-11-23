#pragma once

#include "LEDControl.h"

class LEDOff : LEDMode {
 public:
  LEDOff (void) { LEDControl.mode_add (this); };
};

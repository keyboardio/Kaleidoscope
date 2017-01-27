#pragma once

#include "Keyboardio-LEDControl.h"
#include "LEDUtils.h"

class LEDChaseEffect_ : LEDMode {
 public:
  LEDChaseEffect_ (void);

  virtual void update (void) final;

 private:
  uint8_t pos = 0;
  int8_t chase_sign = 1; //negative values when it's going backwar
  uint8_t chase_pixels = 5;
  uint8_t current_chase_counter = 0;
  static const uint8_t chase_threshold = 20;
};

extern LEDChaseEffect_ LEDChaseEffect;

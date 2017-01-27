#pragma once

#include "Keyboardio-LEDControl.h"
#include "LEDUtils.h"

class LEDBreatheEffect_ : LEDMode {
 public:
  LEDBreatheEffect_ (void);

  virtual void update (void) final;

 private:
};

extern LEDBreatheEffect_ LEDBreatheEffect;

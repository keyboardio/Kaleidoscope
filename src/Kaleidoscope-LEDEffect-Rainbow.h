#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "LEDUtils.h"

class LEDRainbowEffect_ : LEDMode {
 public:
  LEDRainbowEffect_ (void);

  virtual void update (void) final;

 private:
  uint16_t rainbow_hue = 0;   //stores 0 to 614

  static const uint8_t rainbow_steps = 1; //number of hues we skip in a 360 range per update
  long rainbow_current_ticks = 0;
  static const long rainbow_ticks = 10; //delays between update

  static const byte rainbow_saturation = 255;
  static const byte rainbow_value = 50;

};

extern LEDRainbowEffect_ LEDRainbowEffect;

class LEDRainbowWaveEffect_ : LEDMode {
 public:
  LEDRainbowWaveEffect_ (void);

  virtual void update (void) final;

 private:
  uint16_t rainbow_hue = 0;   //stores 0 to 614

  static const uint8_t rainbow_wave_steps = 1; //number of hues we skip in a 360 range per update
  long rainbow_current_ticks = 0;
  static const long rainbow_wave_ticks = 10; //delays between update

  static const byte rainbow_saturation = 255;
  static const byte rainbow_value = 50;
};

extern LEDRainbowWaveEffect_ LEDRainbowWaveEffect;

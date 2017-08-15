#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "LEDUtils.h"

namespace kaleidoscope {
class LEDRainbowEffect : public LEDMode {
 public:
  LEDRainbowEffect(void) {}

  void brightness(byte);
  void update(void) final;

 private:
  uint16_t rainbow_hue = 0;   //  stores 0 to 614

  uint8_t rainbow_steps = 1;  //  number of hues we skip in a 360 range per update
  uint16_t rainbow_current_ticks = 0;
  uint16_t rainbow_ticks = 10;  //  delays between update

  byte rainbow_saturation = 255;
  byte rainbow_value = 50;
};


class LEDRainbowWaveEffect : public LEDMode {
 public:
  LEDRainbowWaveEffect(void) {}

  void brightness(byte);
  void update(void) final;

 private:
  uint16_t rainbow_hue = 0;  //  stores 0 to 614

  uint8_t rainbow_wave_steps = 1;  //  number of hues we skip in a 360 range per update
  uint16_t rainbow_current_ticks = 0;
  uint16_t rainbow_wave_ticks = 10;  //  delays between update

  byte rainbow_saturation = 255;
  byte rainbow_value = 50;
};
}

extern kaleidoscope::LEDRainbowEffect LEDRainbowEffect;
extern kaleidoscope::LEDRainbowWaveEffect LEDRainbowWaveEffect;

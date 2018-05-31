#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "LEDUtils.h"

namespace kaleidoscope {
class LEDRainbowEffect : public LEDMode {
 public:
  LEDRainbowEffect(void) {}

  void brightness(byte);
  byte brightness(void) { return rainbow_value; }
  void update_delay(byte);
  byte update_delay(void) { return rainbow_update_delay; }
  void update(void) final;

 private:
  uint16_t rainbow_hue = 0;   //  stores 0 to 614

  uint8_t rainbow_steps = 1;  //  number of hues we skip in a 360 range per update
  uint16_t rainbow_last_update = 0;
  uint16_t rainbow_update_delay = 40; // delay between updates (ms)

  byte rainbow_saturation = 255;
  byte rainbow_value = 50;
};


class LEDRainbowWaveEffect : public LEDMode {
 public:
  LEDRainbowWaveEffect(void) {}

  void brightness(byte);
  byte brightness(void) { return rainbow_value; }
  void update_delay(byte);
  byte update_delay(void) { return rainbow_update_delay; }
  void update(void) final;

 private:
  uint16_t rainbow_hue = 0;  //  stores 0 to 614

  uint8_t rainbow_wave_steps = 1;  //  number of hues we skip in a 360 range per update
  uint16_t rainbow_last_update = 0;
  uint16_t rainbow_update_delay = 40; // delay between updates (ms)

  byte rainbow_saturation = 255;
  byte rainbow_value = 50;
};
}

extern kaleidoscope::LEDRainbowEffect LEDRainbowEffect;
extern kaleidoscope::LEDRainbowWaveEffect LEDRainbowWaveEffect;

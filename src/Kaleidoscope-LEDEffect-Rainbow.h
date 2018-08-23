/* Kaleidoscope-LEDEffect-Rainbow - Rainbow LED effects for Kaleidoscope.
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "LEDUtils.h"

namespace kaleidoscope {
class LEDRainbowEffect : public LEDMode {
 public:
  LEDRainbowEffect(void) {}

  void brightness(byte);
  byte brightness(void) {
    return rainbow_value;
  }
  void update_delay(byte);
  byte update_delay(void) {
    return rainbow_update_delay;
  }
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
  byte brightness(void) {
    return rainbow_value;
  }
  void update_delay(byte);
  byte update_delay(void) {
    return rainbow_update_delay;
  }
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

/* Kaleidoscope-LEDEffect-Rainbow - Rainbow LED effects for Kaleidoscope.
 * Copyright (C) 2017-2019  Keyboard.io, Inc.
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
#include "Kaleidoscope-UnderglowControl.h"

namespace kaleidoscope {
namespace plugin {

class RainbowEffect {
 public:
  RainbowEffect() {}

  void brightness(byte brightness) {
    brightness_ = brightness;
  }
  byte brightness() {
    return brightness_;
  }
  void update_delay(uint16_t delay) {
    update_delay_ = delay;
  }
  uint16_t update_delay() {
    return update_delay_;
  }

 protected:
  bool updateColor(cRGB &rainbow);

 private:
  uint16_t hue_ = 0;   //  stores 0 to 614

  uint8_t steps_ = 1;  //  number of hues we skip in a 360 range per update
  uint16_t last_update_ = 0;
  uint16_t update_delay_ = 40; // delay between updates (ms)

  byte saturation_ = 255;
  byte brightness_ = 50;
};

class LEDRainbowEffect : public LEDMode, public RainbowEffect {
 public:
  LEDRainbowEffect(void) {}

  void update(void) final;
};

class UnderglowRainbowEffect : public UnderglowEffect, public RainbowEffect {
 public:
  UnderglowRainbowEffect(void) {}

  void update(void) final;
};

// ---

class RainbowWaveEffect {
 public:
  RainbowWaveEffect() {}

  void brightness(byte brightness) {
    brightness_ = brightness;
  }
  byte brightness() {
    return brightness_;
  }
  void update_delay(uint16_t delay) {
    update_delay_ = delay;
  }
  uint16_t update_delay() {
    return update_delay_;
  }

 protected:
  bool updateHue();

  uint16_t hue_ = 0;
  byte saturation_ = 255;
  byte brightness_ = 50;

 private:
  uint8_t steps_ = 1;
  uint16_t last_update_ = 0;
  uint16_t update_delay_ = 40;
};

class LEDRainbowWaveEffect : public LEDMode, public RainbowWaveEffect {
 public:
  LEDRainbowWaveEffect(void) {}

  void update(void) final;
};

class UnderglowRainbowWaveEffect : public UnderglowEffect, public RainbowWaveEffect {
 public:
  UnderglowRainbowWaveEffect(void) {}

  void update(void) final;
};

}
}

extern kaleidoscope::plugin::LEDRainbowEffect LEDRainbowEffect;
extern kaleidoscope::plugin::LEDRainbowWaveEffect LEDRainbowWaveEffect;
extern kaleidoscope::plugin::UnderglowRainbowEffect UnderglowRainbowEffect;
extern kaleidoscope::plugin::UnderglowRainbowWaveEffect UnderglowRainbowWaveEffect;

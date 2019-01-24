/* Kaleidoscope-LEDEffect-Breathe - A breathing effect on the LEDs, for Kaleidoscope.
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
class LEDBreatheEffect : public LEDMode {
 public:
  LEDBreatheEffect(void) {}

  uint8_t hue = 170;
  uint8_t saturation = 255;

 protected:
  void update(void) final;

 private:
  uint16_t last_update_ = 0;
};

class UnderglowBreatheEffect : public UnderglowEffect {
 public:
  UnderglowBreatheEffect() {}

  uint8_t hue() {
    return hue_;
  }
  void hue(uint8_t value) {
    hue_ = value;
  }
  uint8_t saturation() {
    return saturation_;
  }
  void saturation(uint8_t value) {
    saturation_ = value;
  }

 protected:
  void update() final;

 private:
  uint8_t hue_ = 170;
  uint8_t saturation_ = 255;
  uint16_t last_update_ = 0;
};

}
}

extern kaleidoscope::plugin::LEDBreatheEffect LEDBreatheEffect;
extern kaleidoscope::plugin::UnderglowBreatheEffect UnderglowBreatheEffect;

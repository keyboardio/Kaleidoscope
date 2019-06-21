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

#include "Kaleidoscope-LEDEffect-Rainbow.h"

namespace kaleidoscope {
namespace plugin {

void LEDRainbowEffect::TransientLEDMode::update(void) {
  if (!Kaleidoscope.has_leds)
    return;

  uint16_t elapsed = Kaleidoscope.millisAtCycleStart() - rainbow_last_update;
  if (elapsed < parent_->rainbow_update_delay) {
    return;
  } else {
    rainbow_last_update = Kaleidoscope.millisAtCycleStart();
  }

  cRGB rainbow = hsvToRgb(rainbow_hue, rainbow_saturation, parent_->rainbow_value);

  rainbow_hue += rainbow_steps;
  if (rainbow_hue >= 255) {
    rainbow_hue -= 255;
  }
  ::LEDControl.set_all_leds_to(rainbow);
}

void LEDRainbowEffect::brightness(byte brightness) {
  rainbow_value = brightness;
}

void LEDRainbowEffect::update_delay(byte delay) {
  rainbow_update_delay = delay;
}


// ---------

void LEDRainbowWaveEffect::TransientLEDMode::update(void) {
  if (!Kaleidoscope.has_leds)
    return;

  uint16_t elapsed = Kaleidoscope.millisAtCycleStart() - rainbow_last_update;
  if (elapsed < parent_->rainbow_update_delay) {
    return;
  } else {
    rainbow_last_update = Kaleidoscope.millisAtCycleStart();
  }

  for (uint8_t i = 0; i < LED_COUNT; i++) {
    uint16_t key_hue = rainbow_hue + 16 * (i / 4);
    // this needed if LED_COUNT > 128
    if (key_hue >= 511)
      key_hue -= 511;
    if (key_hue >= 255)
      key_hue -= 255;
    cRGB rainbow = hsvToRgb(key_hue, rainbow_saturation, parent_->rainbow_value);
    ::LEDControl.setCrgbAt(i, rainbow);
  }
  rainbow_hue += rainbow_wave_steps;
  if (rainbow_hue >= 255) {
    rainbow_hue -= 255;
  }
}

void LEDRainbowWaveEffect::brightness(byte brightness) {
  rainbow_value = brightness;
}

void LEDRainbowWaveEffect::update_delay(byte delay) {
  rainbow_update_delay = delay;
}

}
}

kaleidoscope::plugin::LEDRainbowEffect LEDRainbowEffect;
kaleidoscope::plugin::LEDRainbowWaveEffect LEDRainbowWaveEffect;

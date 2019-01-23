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

#include "Kaleidoscope-LEDEffect-Rainbow.h"

namespace kaleidoscope {
namespace plugin {

bool RainbowEffect::updateColor(cRGB &rainbow) {
  uint16_t now = millis();
  if ((now - last_update_) < update_delay_) {
    return false;
  } else {
    last_update_ = now;
  }

  rainbow = hsvToRgb(hue_, saturation_, brightness_);

  hue_ += steps_;
  if (hue_ >= 255) {
    hue_ -= 255;
  }

  return true;
}

void LEDRainbowEffect::update() {
  if (!Kaleidoscope.has_leds)
    return;

  cRGB rainbow;
  if (updateColor(rainbow))
    ::LEDControl.set_all_leds_to(rainbow);
}

void UnderglowRainbowEffect::update() {
  if (!::UnderglowControl.has_leds())
    return;

  cRGB rainbow;
  if (!updateColor(rainbow))
    return;

  ::UnderglowControl.setColor(rainbow);
}

// ---------

bool RainbowWaveEffect::updateHue() {
  uint16_t now = millis();
  if ((now - last_update_) < update_delay_) {
    return false;
  } else {
    last_update_ = now;
  }

  hue_ += steps_;
  if (hue_ >= 255) {
    hue_ -= 255;
  }

  return true;
}

void LEDRainbowWaveEffect::update() {
  if (!Kaleidoscope.has_leds)
    return;

  if (!updateHue())
    return;

  for (int8_t i = 0; i < LED_COUNT; i++) {
    uint16_t key_hue = hue_ + 16 * (i / 4);
    if (key_hue >= 255) {
      key_hue -= 255;
    }
    cRGB rainbow = hsvToRgb(key_hue, saturation_, brightness_);
    ::LEDControl.setCrgbAt(i, rainbow);
  }
}

void UnderglowRainbowWaveEffect::update() {
  if (!::UnderglowControl.has_leds())
    return;

  if (!updateHue())
    return;

  for (int8_t i = 0; i < ::UnderglowControl.led_count(); i++) {
    uint16_t led_hue = hue_ + (i * ::UnderglowControl.led_count());
    if (led_hue >= 255) {
      led_hue -= 255;
    }
    cRGB rainbow = hsvToRgb(led_hue, saturation_, brightness_);
    ::UnderglowControl.setColorAt(i, rainbow);
  }
}

}
}

kaleidoscope::plugin::LEDRainbowEffect LEDRainbowEffect;
kaleidoscope::plugin::LEDRainbowWaveEffect LEDRainbowWaveEffect;
kaleidoscope::plugin::UnderglowRainbowEffect UnderglowRainbowEffect;
kaleidoscope::plugin::UnderglowRainbowWaveEffect UnderglowRainbowWaveEffect;

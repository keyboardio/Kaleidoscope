#include "Kaleidoscope-LEDEffect-Rainbow.h"

namespace kaleidoscope {

void LEDRainbowEffect::update(void) {
  uint16_t now = millis();
  if ((now - rainbow_last_update) < rainbow_update_delay) {
    return;
  } else {
    rainbow_last_update = now;
  }

  cRGB rainbow = hsvToRgb(rainbow_hue, rainbow_saturation, rainbow_value);

  rainbow_hue += rainbow_steps;
  if (rainbow_hue >= 255) {
    rainbow_hue -= 255;
  }
  ::LEDControl.set_all_leds_to(rainbow);
}

void LEDRainbowEffect::brightness(byte brightness) {
  rainbow_value = brightness;
}


// ---------

void LEDRainbowWaveEffect::update(void) {
  uint16_t now = millis();
  if ((now - rainbow_last_update) < rainbow_update_delay) {
    return;
  } else {
    rainbow_last_update = now;
  }

  for (uint8_t i = 0; i < LED_COUNT; i++) {
    uint16_t key_hue = rainbow_hue + 16 * (i / 4);
    if (key_hue >= 255)          {
      key_hue -= 255;
    }
    cRGB rainbow = hsvToRgb(key_hue, rainbow_saturation, rainbow_value);
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
}

kaleidoscope::LEDRainbowEffect LEDRainbowEffect;
kaleidoscope::LEDRainbowWaveEffect LEDRainbowWaveEffect;

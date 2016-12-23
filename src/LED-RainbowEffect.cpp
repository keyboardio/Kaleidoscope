#include "LED-RainbowEffect.h"

LEDRainbowEffect_::LEDRainbowEffect_ (void) {
}

void
LEDRainbowEffect_::update (void) {
  if (rainbow_current_ticks++ < rainbow_ticks) {
    return;
  } else {
    rainbow_current_ticks = 0;
  }

  cRGB rainbow = hsv_to_rgb(rainbow_hue, rainbow_saturation, rainbow_value);

  rainbow_hue += rainbow_steps;
  if (rainbow_hue >= 255) {
    rainbow_hue -= 255;
  }
  LEDControl.set_all_leds_to(rainbow);
}

LEDRainbowEffect_ LEDRainbowEffect;

// ---------

LEDRainbowWaveEffect_::LEDRainbowWaveEffect_ (void) {
}

void
LEDRainbowWaveEffect_::update (void) {
  if (rainbow_current_ticks++ < rainbow_wave_ticks) {
    return;
  } else {
    rainbow_current_ticks = 0;
  }

  for (uint8_t i = 0; i < LED_COUNT; i++) {
    uint16_t key_hue = rainbow_hue +16*(i/4);
    if (key_hue >= 255)          {
      key_hue -= 255;
    }
    cRGB rainbow = hsv_to_rgb(key_hue, rainbow_saturation, rainbow_value);
    led_set_crgb_at (i, rainbow);
  }
  rainbow_hue += rainbow_wave_steps;
  if (rainbow_hue >= 255) {
    rainbow_hue -= 255;
  }
}

LEDRainbowWaveEffect_ LEDRainbowWaveEffect;

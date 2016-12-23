#include "LED-Numlock.h"
#include "LEDUtils.h"

static uint8_t numpadIndex;
static uint8_t storedLEDMode;
static uint8_t us;

LEDNumlock::LEDNumlock (uint8_t numpadIdx) {
  numpadIndex = numpadIdx;

  breathState.brightness = 0;
  breathState.fadeAmount = 1;
}

void
LEDNumlock::begin (void) {
  us = LEDControl.mode_add (this);
  loop_hook_add (this->loopHook);
}

void
LEDNumlock::setup (void) {
  if (temporary_keymap != numpadIndex) {
    LEDControl.next_mode ();
  }
}

void
LEDNumlock::update (void) {
  for (uint8_t i = 0; i < 44; i++) {
    led_set_crgb_at(i, {0, 0, 0});
  }
  for (uint8_t i = 44; i < LED_COUNT; i++) {
    led_set_crgb_at(i, {255, 0, 0});
  }

  cRGB color = breath_compute (&breathState);
  led_set_crgb_at (60, color);
}

void
LEDNumlock::loopHook (void) {
  if (temporary_keymap == numpadIndex) {
    if (storedLEDMode != us) {
      storedLEDMode = LEDControl.get_mode ();
    }
    LEDControl.set_mode (us);
  }

  if (temporary_keymap != numpadIndex &&
      LEDControl.get_mode () == us) {
    LEDControl.set_mode (storedLEDMode);
  }
}

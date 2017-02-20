#include "Kaleidoscope-LEDControl.h"

LEDMode *LEDControl_::modes[LED_MAX_MODES];
uint8_t LEDControl_::previousMode, LEDControl_::mode;
uint16_t LEDControl_::syncDelay = 16;
uint32_t LEDControl_::syncTimer;

void
LEDMode::activate (void) {
  LEDControl.activate (this);
}

void
LEDMode::begin(void) {
  Kaleidoscope.use(&LEDControl, NULL);
  LEDControl.mode_add(this);
}

LEDControl_::LEDControl_(void) {
  mode = previousMode = 0;
  memset (modes, 0, LED_MAX_MODES * sizeof (modes[0]));
}

void
LEDControl_::next_mode (void) {
  mode++;

  if (mode >= LED_MAX_MODES) {
    mode = 0;
    return;
  }

  if (modes[mode])
    return;

  mode = 0;
}

void
LEDControl_::update (void) {
  if (previousMode != mode) {
    set_all_leds_to ({0, 0, 0});
    if (modes[mode])
      (modes[mode]->init) ();
  }

  if (modes[mode])
    (modes[mode]->update) ();

  previousMode = mode;
}

void
LEDControl_::set_mode (uint8_t mode_) {
  if (mode_ < LED_MAX_MODES)
    mode = mode_;
}

uint8_t
LEDControl_::get_mode (void) {
  return mode;
}

void
LEDControl_::activate (LEDMode *mode) {
  for (uint8_t i = 0; i < LED_MAX_MODES; i++) {
    if (modes[i] == mode)
      return set_mode(i);
  }
}

int8_t
LEDControl_::mode_add (LEDMode *mode) {
  for (int i = 0; i < LED_MAX_MODES; i++) {
    if (modes[i])
      continue;

    modes[i] = mode;
    return i;
  }
  return -1;
}

void
LEDControl_::set_all_leds_to(uint8_t r, uint8_t g, uint8_t b) {
  cRGB color;
  color.r=r;
  color.g=g;
  color.b=b;
  set_all_leds_to(color);
}

void
LEDControl_::set_all_leds_to(cRGB color) {
  for (uint8_t i = 0; i < LED_COUNT; i++) {
    led_set_crgb_at(i, color);
  }
}

void
LEDControl_::led_set_crgb_at(uint8_t i, cRGB crgb) {
  KeyboardHardware.led_set_crgb_at(i, crgb);
}

void
LEDControl_::led_set_crgb_at(byte row, byte col, cRGB color) {
  KeyboardHardware.led_set_crgb_at(row, col, color);
}

cRGB
LEDControl_::led_get_crgb_at(uint8_t i) {
  return KeyboardHardware.led_get_crgb_at(i);
}

void
LEDControl_::led_sync(void) {
  KeyboardHardware.led_sync();
}

void
LEDControl_::begin (void) {
  set_all_leds_to ({0, 0, 0});

  for (uint8_t i = 0; i < LED_MAX_MODES; i++) {
    if (modes[i])
      (modes[i]->setup)();
  }

  event_handler_hook_use(eventHandler);
  loop_hook_use(loopHook);

  syncTimer = millis();
}

Key
LEDControl_::eventHandler (Key mappedKey, byte row, byte col, uint8_t keyState) {
  if (mappedKey.flags != (SYNTHETIC | IS_INTERNAL | LED_TOGGLE))
    return mappedKey;

  if (key_toggled_on(keyState))
    LEDControl.next_mode();

  return Key_NoKey;
}

void
LEDControl_::loopHook (bool postClear) {
  if (postClear)
    return;

  if (millis() - syncTimer >= syncDelay) {
    led_sync();
    syncTimer = millis();
  }
  update();
}

LEDControl_ LEDControl;

#include "LEDControl.h"

void
LEDMode::activate (void) {
  LEDControl.activate (this);
}

void
LEDMode::begin(void) {
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
LEDControl_::setup (void) {
  set_all_leds_to ({0, 0, 0});

  for (uint8_t i = 0; i < LED_MAX_MODES; i++) {
    if (modes[i])
      (modes[i]->setup) ();
  }
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

LEDControl_ LEDControl;

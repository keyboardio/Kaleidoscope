#include "Keyboardio-LEDEffect-Numlock.h"
#include "LEDUtils.h"
#include "KeyboardioFirmware.h"
#include "layers.h"

static uint8_t numpadIndex;
static uint8_t storedLEDMode;
static uint8_t us;

LEDNumlock::LEDNumlock (uint8_t numpadIdx) {
    numpadIndex = numpadIdx;
}

void
LEDNumlock::begin (void) {
  us = LEDControl.mode_add (this);
  loop_hook_add (this->loopHook);
}

void
LEDNumlock::init (void) {
    if (!Layer.isOn (numpadIndex)) {
        LEDControl.next_mode();
    }
}

void
LEDNumlock::update (void) {
    for (uint8_t i = 0; i < 44; i++) {
        LEDControl.led_set_crgb_at(i, {0, 0, 0});
    }
    for (uint8_t i = 44; i < LED_COUNT; i++) {
        LEDControl.led_set_crgb_at(i, {255, 0, 0});
    }

    cRGB color = breath_compute ();
    LEDControl.led_set_crgb_at (60, color);
}

void
LEDNumlock::loopHook (bool postClear) {
    if (!postClear)
        return;

    if (!Layer.isOn (numpadIndex)) {
        if (LEDControl.get_mode () != us)
            storedLEDMode = LEDControl.get_mode ();
        LEDControl.set_mode (storedLEDMode);
    } else {
        LEDControl.set_mode (us);
    }
}

#include "Kaleidoscope-Numlock.h"
#include "LEDUtils.h"
#include "Kaleidoscope.h"
#include "layers.h"

bool NumLock_::isActive;
byte NumLock_::row = 255, NumLock_::col = 255;
cRGB numpad_color = CRGB(255, 0, 0);


NumLock_::NumLock_(void) {
}

void NumLock_::begin(void) {
  loop_hook_use(loopHook);
}

void NumLock_::loopHook(bool postClear) {
  if (!postClear || !isActive)
    return;

  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {
      Key k = Layer.lookup(r, c);

      if (k.raw < Key_KeypadNumLock.raw || k.raw > Key_KeypadDot.raw)
        continue;

      LEDControl.led_set_crgb_at(r, c, numpad_color);
    }
  }

  if (row > ROWS || col > COLS)
    return;

  cRGB color = breath_compute();
  LEDControl.led_set_crgb_at(row, col, color);
}

const macro_t *NumLock_::toggle(byte row_, byte col_, uint8_t numPadLayer) {
  row = row_;
  col = col_;

  if (Layer.isOn(numPadLayer)) {
    Layer.off(numPadLayer);
  } else {
    Layer.on(numPadLayer);
  }
  isActive = Layer.isOn(numPadLayer);

  return MACRO(T(KeypadNumLock), END);
}

NumLock_ NumLock;

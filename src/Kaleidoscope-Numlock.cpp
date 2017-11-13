#include "Kaleidoscope-Numlock.h"
#include "LEDUtils.h"
#include "Kaleidoscope.h"
#include "layers.h"

byte NumLock_::row = 255, NumLock_::col = 255;
uint8_t NumLock_::numPadLayer;
bool NumLock_::cleanupDone = true;
cRGB numpad_color = CRGB(255, 0, 0);

void NumLock_::begin(void) {
  Kaleidoscope.useLoopHook(loopHook);
}

void NumLock_::loopHook(bool postClear) {
  if (!postClear)
    return;

  if (!Layer.isOn(numPadLayer)) {
    if (!cleanupDone) {
      LEDControl.set_mode(LEDControl.get_mode_index());
      cleanupDone = true;
    }
    return;
  }

  cleanupDone = false;
  bool numState = !!(kaleidoscope::hid::getKeyboardLEDs() & LED_NUM_LOCK);
  if (!numState) {
    kaleidoscope::hid::pressKey(Key_KeypadNumLock);
    kaleidoscope::hid::sendKeyboardReport();
  }

  LEDControl.set_mode(LEDControl.get_mode_index());

  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {
      Key k = Layer.lookupOnActiveLayer(r, c);
      Key layer_key = Layer.getKey(numPadLayer, r, c);

      if (k == LockLayer(numPadLayer)) {
        row  = r;
        col = c;
      }

      if ((k != layer_key) || (k.flags != KEY_FLAGS)) {
        LEDControl.refreshAt(r, c);
      } else {
        LEDControl.setCrgbAt(r, c, numpad_color);
      }
    }
  }

  if (row > ROWS || col > COLS)
    return;

  cRGB color = breath_compute();
  LEDControl.setCrgbAt(row, col, color);
}

NumLock_ NumLock;

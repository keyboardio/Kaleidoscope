#include "Kaleidoscope-NumPad.h"
#include "LEDUtils.h"
#include "Kaleidoscope.h"
#include "layers.h"

byte NumPad_::row = 255, NumPad_::col = 255;
uint8_t NumPad_::numPadLayer;
bool NumPad_::cleanupDone = true;
bool NumPad_::originalNumLockState = false;
cRGB numpad_color = CRGB(255, 0, 0);

void NumPad_::begin(void) {
  Kaleidoscope.useLoopHook(loopHook);
  originalNumLockState = !!(kaleidoscope::hid::getKeyboardLEDs() & LED_NUM_LOCK);
}

static void syncNumlock(bool state) {
  bool numState = !!(kaleidoscope::hid::getKeyboardLEDs() & LED_NUM_LOCK);
  if (numState != state) {
    kaleidoscope::hid::pressKey(Key_KeypadNumLock);
  }
}

void NumPad_::loopHook(bool postClear) {
  if (!postClear)
    return;

  if (!Layer.isOn(numPadLayer)) {
    bool numState = !!(kaleidoscope::hid::getKeyboardLEDs() & LED_NUM_LOCK);
    if (!cleanupDone) {
      LEDControl.set_mode(LEDControl.get_mode_index());
      syncNumlock(false);
      cleanupDone = true;

      if (numState && !originalNumLockState) {
        kaleidoscope::hid::pressKey(Key_KeypadNumLock);
        numState = false;
      }
    }
    originalNumLockState = numState;
    return;
  }

  cleanupDone = false;
  syncNumlock(true);

  LEDControl.set_mode(LEDControl.get_mode_index());

  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {
      Key k = Layer.lookupOnActiveLayer(r, c);
      Key layer_key = Layer.getKey(numPadLayer, r, c);

      if (k == LockLayer(numPadLayer)) {
        row  = r;
        col = c;
      }

      if ((k != layer_key) || (k == Key_NoKey) || (k.flags != KEY_FLAGS)) {
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

NumPad_ NumPad;

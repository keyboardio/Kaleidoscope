#include "Kaleidoscope-Numlock.h"
#include "LEDUtils.h"
#include "Kaleidoscope.h"
#include "layers.h"

byte NumLock_::row = 255, NumLock_::col = 255;
uint8_t NumLock_::numPadLayer;
cRGB numpad_color = CRGB(255, 0, 0);

void NumLock_::begin(void) {
  Kaleidoscope.useLoopHook(loopHook);
  Kaleidoscope.useEventHandlerHook(eventHandlerHook);
}

void NumLock_::loopHook(bool postClear) {
  if (!postClear || !Layer.isOn(numPadLayer))
    return;

  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {
      Key k = Layer.lookupOnActiveLayer(r, c);
      Key layer_key = Layer.getKey(numPadLayer, r, c);

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

Key NumLock_::eventHandlerHook(Key key, byte row, byte col, uint8_t key_state) {
  if (key != Key_KeypadNumLock)
    return key;

  if (!key_toggled_on(key_state))
    return key;

  if (Layer.isOn(numPadLayer)) {
    Layer.off(numPadLayer);
    LEDControl.set_mode(LEDControl.get_mode_index());
  } else {
    Layer.on(numPadLayer);
  }

  return key;
}

NumLock_ NumLock;

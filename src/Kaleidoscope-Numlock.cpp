#include "Kaleidoscope-Numlock.h"
#include "LEDUtils.h"
#include "Kaleidoscope.h"
#include "layers.h"

byte NumLock_::row = 255, NumLock_::col = 255;
uint8_t NumLock_::numPadLayer;
cRGB numpad_color = CRGB(255, 0, 0);

void NumLock_::begin(void) {
  Kaleidoscope.useLoopHook(loopHook);
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

const macro_t *NumLock_::toggle() {
  row = Macros.row;
  col = Macros.col;

  if (Layer.isOn(numPadLayer)) {
    Layer.off(numPadLayer);
    LEDControl.set_mode(LEDControl.get_mode_index());
  } else {
    Layer.on(numPadLayer);
  }

  return MACRO(T(KeypadNumLock), END);
}

NumLock_ NumLock;

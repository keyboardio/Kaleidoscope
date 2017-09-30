#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-Macros.h"
#include "LEDUtils.h"

class NumLock_ : public KaleidoscopePlugin {
 public:
  NumLock_(void) {}

  void begin(void) final;

  static uint8_t numPadLayer;

 private:
  static void loopHook(const bool postClear);
  static Key eventHandlerHook(Key key, byte row, byte col, uint8_t key_state);

  static byte row, col;
};

extern NumLock_ NumLock;

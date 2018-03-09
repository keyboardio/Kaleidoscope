#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-Macros.h"
#include "LEDUtils.h"

class NumPad_ : public KaleidoscopePlugin {
 public:
  NumPad_(void) {}

  void begin(void) final;

  static uint8_t numPadLayer;

 private:
  static void loopHook(const bool postClear);

  static byte row, col;
  static bool cleanupDone;
  static bool originalNumLockState;
};

extern NumPad_ NumPad;

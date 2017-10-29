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

  static byte row, col;
  static bool isOn;
};

extern NumLock_ NumLock;

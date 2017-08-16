#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-Macros.h"
#include "LEDUtils.h"

#define TOGGLENUMLOCK 0
#define Key_ToggleNumlock M(TOGGLENUMLOCK)

class NumLock_ : public KaleidoscopePlugin {
 public:
  NumLock_(void) {}

  void begin(void) final;

  static const macro_t *toggle();
  static void loopHook(const bool postClear);

  static uint8_t numPadLayer;

 private:
  static byte row, col;
};

extern NumLock_ NumLock;

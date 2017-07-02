#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-Macros.h"
#include "LEDUtils.h"

#define TOGGLENUMLOCK 0
#define Key_ToggleNumlock M(TOGGLENUMLOCK)

class NumLock_ : public KaleidoscopePlugin {
 public:
  NumLock_(void);

  void begin(void) final;

  static const macro_t *toggle(byte row, byte col, uint8_t numPadLayer);
  static void loopHook(const bool postClear);

 private:
  static bool isActive;
  static byte row, col;
};

extern NumLock_ NumLock;

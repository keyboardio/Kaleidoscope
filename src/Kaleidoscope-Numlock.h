#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-Macros.h"
#include "LEDUtils.h"

#define TOGGLENUMLOCK 0
#define Key_ToggleNumlock M(TOGGLENUMLOCK)

class NumLock_ : public LEDMode {
 public:
  NumLock_(void);

  void begin(void) final;

  void update(void) final;
  void init(void) final;

  static const macro_t *toggle(byte row, byte col, uint8_t numPadLayer);

 private:
  static uint8_t previousLEDMode;
  static uint8_t us;
  static bool isActive;
  static byte row, col;
};

extern NumLock_ NumLock;

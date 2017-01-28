#pragma once

#include "Keyboardio-LEDControl.h"
#include "Keyboardio-Macros.h"
#include "LEDUtils.h"

class NumLock_ : LEDMode {
 public:
  NumLock_ (void);

  virtual void begin (void) final;

  virtual void update (void) final;
  virtual void init (void) final;

  static const macro_t *toggle (byte row, byte col, uint8_t numPadLayer);

 private:
  static uint8_t previousLEDMode;
  static uint8_t us;
  static bool isActive;
  static byte row, col;
};

extern NumLock_ NumLock;

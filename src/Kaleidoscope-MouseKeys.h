#pragma once

#include "Kaleidoscope.h"
#include "MouseKeyDefs.h"

class MouseKeys_ : public KaleidoscopePlugin {
 public:
  MouseKeys_(void);

  virtual void begin(void) final;

  static uint8_t speed;
  static uint16_t speedDelay;
  static uint8_t accelSpeed;
  static uint16_t accelDelay;
  static uint8_t wheelSpeed;
  static uint16_t wheelDelay;

 private:
  static uint8_t mouseMoveIntent;
  static uint32_t endTime;
  static uint32_t accelEndTime;
  static uint32_t wheelEndTime;

  static void scrollWheel(uint8_t keyCode);
  static void loopHook(bool postClear);
  static Key eventHandlerHook(Key mappedKey, byte row, byte col, uint8_t keyState);
};

extern MouseKeys_ MouseKeys;

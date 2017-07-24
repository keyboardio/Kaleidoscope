#pragma once

#include <Arduino.h>
#include "Kaleidoscope.h"

#define TEST_MODE_KEY_COMBO (R0C0 | R0C6 | R3C6)

class TestMode_ : public KaleidoscopePlugin {
 public:
  TestMode_(void);
  void begin();

 private:
  static void run_tests();
  static void test_leds();
  static void testMatrix();
  static void toggle_programming_leds_on();
  static void waitForKeypress();
  static void loopHook(bool postClear);
  static void set_leds(uint8_t r, uint8_t g, uint8_t b);
};

extern TestMode_ TestMode;

Key handleKeyswitchEvent_test(Key mappedKey, byte row, byte col, uint8_t keyState);

#pragma once

#include <Arduino.h>
#include "Kaleidoscope.h"

class TestMode_ : public KaleidoscopePlugin {
  public:
    TestMode_(void);
    void begin();

  private:
    static void run_tests();
    static void test_leds();
    static void test_matrix();
    static void wait_for_keypress();
    static void loopHook(bool postClear);
    static void set_leds(uint8_t r, uint8_t g, uint8_t b);
};

extern TestMode_ TestMode;

Key handle_key_event_test(Key mappedKey, byte row, byte col, uint8_t keyState);

#pragma once

#include <Arduino.h>
#include "KeyboardioFirmware.h"

class TestMode_ : public KeyboardioPlugin {
  public:
    TestMode_(void);
    void begin();

  private:
    static void run_tests();
    static void test_leds();
    static void test_matrix();
    static void loopHook(bool postClear);
};

extern TestMode_ TestMode;

Key handle_key_event_test(Key mappedKey, byte row, byte col, uint8_t keyState);

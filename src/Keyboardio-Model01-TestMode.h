#pragma once
#include <Arduino.h>

class TestMode_ {
  public:
    TestMode_(void);
    void setup();
    static void loop();
    void begin();

  private:
    static void TestLEDs();
    static void TestKeys();
    static void TestMatrix();
    static void TestOneKey();
    static void loopHook(bool postClear);
};

extern TestMode_ TestMode;

Key handle_key_event_test(Key mappedKey, byte row, byte col, uint8_t keyState);

#pragma once
#include <Arduino.h>

class TestMode_ {
  public:
    TestMode_(void);
    void setup();
    void loop();

  private:
    void TestLEDs();
    void TestKeys();
    void TestMatrix();
    void TestOneKey();
};

extern TestMode_ TestMode;

bool handle_key_event_test(Key mappedKey, byte row, byte col, uint8_t keyState);

#pragma once
#include <Arduino.h>

class TestMode_ {
  public:
    TestMode_(void);
    void setup();

    void TestLEDs();
    void TestKeys();
    void TestMatrix();
    void TestOneKey();
  private:
};

extern TestMode_ TestMode;

bool handle_key_event_test(byte row, byte col, uint8_t currentState, uint8_t previousState);

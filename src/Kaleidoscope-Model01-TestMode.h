#pragma once

#include <Arduino.h>
#include "Kaleidoscope.h"

typedef struct {
  uint8_t cyclesSinceStateChange[32];
  uint32_t badKeys;

} side_data_t;
class TestMode_ : public kaleidoscope::Plugin {
 public:
  TestMode_(void) {};

  kaleidoscope::EventHandlerResult beforeReportingState();

#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
 protected:
  void begin();
  static void legacyLoopHook(bool is_post_clear);
#endif

 private:
  static void run_tests();
  static void test_leds();
  static void testMatrix();
  static void toggle_programming_leds_on();
  static void handleKeyEvent(side_data_t *side, keydata_t *oldState, keydata_t *newState, uint8_t row, uint8_t col, uint8_t col_offset);
  static void waitForKeypress();
  static void set_leds(cRGB color);
};

extern TestMode_ TestMode;

Key handleKeyswitchEvent_test(Key mappedKey, byte row, byte col, uint8_t keyState);

/* Kaleidoscope-Model01-TestMode - A factory test mode for the Model 01.
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#ifdef ARDUINO_AVR_MODEL01

#include <Arduino.h>
#include "Kaleidoscope.h"

namespace kaleidoscope {
namespace plugin {

class TestMode : public kaleidoscope::Plugin {
 public:
  typedef struct {
    uint8_t cyclesSinceStateChange[32];
    uint32_t badKeys;

  } side_data_t;

  TestMode(void) {};

  EventHandlerResult beforeReportingState();

 private:
  static void run_tests();
  static void test_leds();
  static void testMatrix();
  static void toggle_programming_leds_on();
  static void handleKeyEvent(side_data_t *side, keydata_t *oldState, keydata_t *newState, uint8_t row, uint8_t col, uint8_t col_offset);
  static void waitForKeypress();
  static void set_leds(cRGB color);
};
}
}

extern kaleidoscope::plugin::TestMode TestMode;

#endif

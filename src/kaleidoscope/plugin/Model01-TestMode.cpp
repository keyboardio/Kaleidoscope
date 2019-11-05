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

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD

#if ARDUINO_AVR_MODEL01

#include "Kaleidoscope.h"
#include "Kaleidoscope-Model01-TestMode.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"
#include "kaleidoscope_internal/LEDModeManager.h"

namespace kaleidoscope {
namespace plugin {

constexpr uint8_t CHATTER_CYCLE_LIMIT = 30;
constexpr uint8_t TOGGLED_OFF = 2;
constexpr uint8_t TOGGLED_ON = 1;
constexpr uint8_t HELD = 3;
constexpr uint8_t RELEASED = 0;

EventHandlerResult TestMode::beforeReportingState() {
  if (Kaleidoscope.device().isKeyswitchPressed(R0C0) &&
      Kaleidoscope.device().isKeyswitchPressed(R0C6) &&
      Kaleidoscope.device().isKeyswitchPressed(R3C6) &&
      Kaleidoscope.device().pressedKeyswitchCount() == 3) {
    run_tests();
  }
  return EventHandlerResult::OK;
}

void TestMode::waitForKeypress() {
  for (uint8_t temp = 0; temp < 8; temp++) {
    Kaleidoscope.device().readMatrix();
  }
  while (1) {
    Kaleidoscope.device().readMatrix();
    if (Kaleidoscope.device().isKeyswitchPressed(R3C6)
        && Kaleidoscope.device().pressedKeyswitchCount() == 1
        && Kaleidoscope.device().previousLeftHandState.all == 0) {
      break;
    }
  }
}

void TestMode::set_leds(cRGB color) {
  ::LEDControl.set_all_leds_to(color);
  ::LEDControl.syncLeds();
  waitForKeypress();
}

void TestMode::test_leds(void) {
  constexpr cRGB red = CRGB(201, 0, 0);
  constexpr cRGB blue = CRGB(0, 0, 201);
  constexpr cRGB green = CRGB(0, 201, 0);
  constexpr cRGB brightWhite = CRGB(160, 160, 160);

  // make all the LEDs bright red
  set_leds(red);
  // make all the LEDs bright green
  set_leds(green);
  // make all the LEDs bright blue
  set_leds(blue);
  // make all the LEDs bright white (1.6A)
  set_leds(brightWhite);

  // This works under the assumption that LEDRainbowEffect
  // has been registered with KALEIDOSCOPE_INIT_PLUGINS in
  // the sketch. Otherwise LEDRainbowEffect would not be
  // known to LEDControl.
  //
  ::LEDControl.activate(&::LEDRainbowEffect);

  // rainbow for 10 seconds
  for (auto i = 0; i < 1000; i++) {
    ::LEDControl.update();
    ::LEDControl.syncLeds();
  }
  waitForKeypress();
}



void TestMode::handleKeyEvent(side_data_t *side, keydata_t *oldState, keydata_t *newState, KeyAddr key_addr, uint8_t col_offset) {

  constexpr cRGB red = CRGB(201, 0, 0);
  constexpr cRGB blue = CRGB(0, 0, 201);
  constexpr cRGB green = CRGB(0, 201, 0);

  auto keynum = key_addr.toInt();

  const uint8_t keyState = ((bitRead(oldState->all, keynum) << 1) |
                            (bitRead(newState->all, keynum) << 0));
  if (keyState == TOGGLED_ON) {
    if (side->cyclesSinceStateChange[keynum] < CHATTER_CYCLE_LIMIT) {
      bitSet(side->badKeys, keynum);
    }
    side->cyclesSinceStateChange[keynum] = 0;
  } else if (side->cyclesSinceStateChange[keynum] <= CHATTER_CYCLE_LIMIT)  {
    side->cyclesSinceStateChange[keynum]++;
  }

  auto key_addr_col_shifted = KeyAddr(key_addr.row(), col_offset - key_addr.col());

  // If the key is held down
  if (keyState == HELD) {
    Kaleidoscope.device().setCrgbAt(key_addr_col_shifted, green);
  } else if (bitRead(side->badKeys, key_addr_col_shifted.toInt()) == 1) {
    // If we triggered chatter detection ever on this key
    Kaleidoscope.device().setCrgbAt(key_addr_col_shifted, red);
  } else if (keyState == TOGGLED_OFF) {
    // If the key was just released
    Kaleidoscope.device().setCrgbAt(key_addr_col_shifted, blue);
  }
}


void TestMode::testMatrix() {
  // Reset bad keys from previous tests.
  side_data_t left = {{0}, 0};
  side_data_t right = {{0}, 0};


  ::LEDControl.set_all_leds_to(200, 0, 0);
  // Clear out the key event buffer so we don't get messed up information from
  // taps during LED test mode.
  while (1) {
    Kaleidoscope.device().readMatrix();
    if (Kaleidoscope.device().isKeyswitchPressed(R0C0) &&
        Kaleidoscope.device().isKeyswitchPressed(R0C6) &&
        Kaleidoscope.device().isKeyswitchPressed(R3C6) &&
        Kaleidoscope.device().pressedKeyswitchCount() == 3) {
      break;
    }
    for (auto key_addr : KeyAddr::all()) {
      handleKeyEvent(&left, &(Kaleidoscope.device().previousLeftHandState), &(Kaleidoscope.device().leftHandState), key_addr, 7);
      handleKeyEvent(&right, &(Kaleidoscope.device().previousRightHandState), &(Kaleidoscope.device().rightHandState), key_addr, 15);
    }
    ::LEDControl.syncLeds();
  }
}

void TestMode::toggle_programming_leds_on() {
  PORTD |= (1 << 5);
  PORTB |= (1 << 0);
}

void TestMode::run_tests() {
  //  Kaleidoscope.device().serialPort().println("Running tests");
  toggle_programming_leds_on();
  // Disable debouncing
  Kaleidoscope.device().setKeyscanInterval(2);
  test_leds();
  testMatrix();
  //  Kaleidoscope.device().serialPort().println("Done running tests");
}

}
}

kaleidoscope::plugin::TestMode TestMode;

#endif

#endif

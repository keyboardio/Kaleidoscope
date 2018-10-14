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

#include "Kaleidoscope.h"
#include "Kaleidoscope-Model01-TestMode.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"

namespace kaleidoscope {
namespace plugin {

constexpr uint8_t CHATTER_CYCLE_LIMIT = 30;
constexpr uint8_t TOGGLED_OFF = 2;
constexpr uint8_t TOGGLED_ON = 1;
constexpr uint8_t HELD = 3;
constexpr uint8_t RELEASED = 0;

EventHandlerResult TestMode::beforeReportingState() {
  if (KeyboardHardware.isKeyswitchPressed(R0C0) &&
      KeyboardHardware.isKeyswitchPressed(R0C6) &&
      KeyboardHardware.isKeyswitchPressed(R3C6) &&
      KeyboardHardware.pressedKeyswitchCount() == 3) {
    run_tests();
  }
  return EventHandlerResult::OK;
}

void TestMode::waitForKeypress() {
  for (uint8_t temp = 0; temp < 8; temp++) {
    KeyboardHardware.readMatrix();
  }
  while (1) {
    KeyboardHardware.readMatrix();
    if (KeyboardHardware.isKeyswitchPressed(R3C6)
        && KeyboardHardware.pressedKeyswitchCount() == 1
        && KeyboardHardware.previousLeftHandState.all == 0) {
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
  // rainbow for 10 seconds
  for (auto i = 0; i < 1000; i++) {
    ::LEDRainbowEffect.update();
    ::LEDControl.syncLeds();
  }
  waitForKeypress();
}



void TestMode::handleKeyEvent(side_data_t *side, keydata_t *oldState, keydata_t *newState, uint8_t row, uint8_t col, uint8_t col_offset) {

  constexpr cRGB red = CRGB(201, 0, 0);
  constexpr cRGB blue = CRGB(0, 0, 201);
  constexpr cRGB green = CRGB(0, 201, 0);

  const uint8_t keynum = (row * 8) + (col);

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



  // If the key is held down
  if (keyState == HELD) {
    KeyboardHardware.setCrgbAt(row, col_offset - col, green);
  }
  // If we triggered chatter detection ever on this key
  else if (bitRead(side->badKeys, keynum) == 1) {
    KeyboardHardware.setCrgbAt(row, col_offset - col, red);
  }

  // If the key was just released
  else if (keyState == TOGGLED_OFF) {
    KeyboardHardware.setCrgbAt(row, col_offset - col, blue);
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
    KeyboardHardware.readMatrix();
    if (KeyboardHardware.isKeyswitchPressed(R0C0) &&
        KeyboardHardware.isKeyswitchPressed(R0C6) &&
        KeyboardHardware.isKeyswitchPressed(R3C6) &&
        KeyboardHardware.pressedKeyswitchCount() == 3) {
      break;
    }
    for (byte row = 0; row < 4; row++) {
      for (byte col = 0; col < 8; col++) {
        handleKeyEvent(&left, &(KeyboardHardware.previousLeftHandState), &(KeyboardHardware.leftHandState), row, col, 7);
        handleKeyEvent(&right, &(KeyboardHardware.previousRightHandState), &(KeyboardHardware.rightHandState), row, col, 15);
      }
    }
    ::LEDControl.syncLeds();
  }
}

void TestMode::toggle_programming_leds_on() {
  PORTD |= (1 << 5);
  PORTB |= (1 << 0);
}

void TestMode::run_tests() {
  //  Serial.println("Running tests");
  toggle_programming_leds_on();
  // Disable debouncing
  KeyboardHardware.setKeyscanInterval(2);
  test_leds();
  testMatrix();
  //  Serial.println("Done running tests");
}

}
}

kaleidoscope::plugin::TestMode TestMode;

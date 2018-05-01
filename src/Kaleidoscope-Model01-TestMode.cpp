#include "Kaleidoscope.h"
#include "Kaleidoscope-Model01-TestMode.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"
#include <avr/io.h>
#include <avr/wdt.h>


#define CHATTER_CYCLE_LIMIT 30
#define TOGGLED_OFF 2
#define TOGGLED_ON 1
#define HELD 3
#define RELEASED 0


cRGB red;
cRGB blue;
cRGB green;
cRGB white;

TestMode_::TestMode_(void) {
}


void TestMode_::begin(void) {
  red.r = 201;
  blue.b = 201;
  green.g = 201;
  white.r = 50;
  white.g = 50;
  white.b = 50;
  Kaleidoscope.useLoopHook(this->loopHook);
}

void TestMode_::loopHook(bool postClear) {
  if (postClear)
    return;
  if (KeyboardHardware.leftHandState.all == TEST_MODE_KEY_COMBO
//  && KeyboardHardware.rightHandState.all == combo.rightHand
     ) {
    run_tests();
  }
}

void TestMode_::waitForKeypress() {
  for (auto temp = 0; temp < 16; temp++) {
    KeyboardHardware.readMatrix();
  }
  while (1) {
    KeyboardHardware.readMatrix();
    if (KeyboardHardware.leftHandState.all ==  R3C6
        && KeyboardHardware.previousLeftHandState.all == 0) {
      break;
    }
  }
}

void TestMode_::set_leds(uint8_t r, uint8_t g, uint8_t b) {
  LEDControl.set_all_leds_to(r, g, b);
  LEDControl.syncLeds();
  waitForKeypress();
}

void TestMode_::test_leds(void) {
  // make all the LEDs bright red
  set_leds(200, 0, 0);
  // make all the LEDs bright green
  set_leds(0, 200, 0);
  // make all the LEDs bright blue
  set_leds(0, 0, 200);
  // make all the LEDs bright white (1.6A)
  set_leds(160, 160, 160);
  // rainbow for 10 seconds
  for (auto i = 0; i < 1000; i++) {
    LEDRainbowEffect.update();
    LEDControl.syncLeds();
  }
  waitForKeypress();
}



void TestMode_::handleKeyEvent(side_data_t *side, keydata_t oldState, keydata_t newState, uint8_t row, uint8_t col, uint8_t col_offset) {
  uint8_t keynum = (row * 8) + (col);

  uint8_t keyState = ((bitRead(oldState.all, keynum) << 1) |
                      (bitRead(newState.all,         keynum) << 0));
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


void TestMode_::testMatrix() {
  // Reset bad keys from previous tests.
  side_data_t left = {{0}, 0};
  side_data_t right = {{0}, 0};


  LEDControl.set_all_leds_to(200, 0, 0);
  // Clear out the key event buffer so we don't get messed up information from
  // taps during LED test mode.
  while (1) {
    KeyboardHardware.readMatrix();
    if (KeyboardHardware.leftHandState.all == TEST_MODE_KEY_COMBO) {
      break;
    }
    for (byte row = 0; row < 4; row++) {
      for (byte col = 0; col < 8; col++) {
        handleKeyEvent(&left, KeyboardHardware.previousLeftHandState, KeyboardHardware.leftHandState, row, col, 7);
        handleKeyEvent(&right, KeyboardHardware.previousRightHandState, KeyboardHardware.leftHandState, row, col, 15);
      }
    }
    LEDControl.syncLeds();
  }
}

void TestMode_::toggle_programming_leds_on() {
  PORTD |= (1 << 5);
  PORTB |= (1 << 0);
}

void TestMode_::run_tests() {
  //  Serial.println("Running tests");
  toggle_programming_leds_on();
  // Disable debouncing
  KeyboardHardware.setKeyscanInterval(2);
  test_leds();
  testMatrix();
  //  Serial.println("Done running tests");
}

TestMode_ TestMode;

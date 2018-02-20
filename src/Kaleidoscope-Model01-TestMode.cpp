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
  delay(2);
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
  // set all the keys to red
}


uint32_t leftBadKeys;
uint32_t rightBadKeys;



uint32_t cyclesSinceStateChange[64];



void TestMode_::testMatrix() {
  LEDControl.set_all_leds_to(200, 0, 0);
  // Clear out the key event buffer so we don't get messed up information from
  // taps during LED test mode.
  for (auto temp = 0; temp < 16; temp++) {
    KeyboardHardware.readMatrix();
  }
  // Reset bad keys from previous tests.
  leftBadKeys = 0;
  rgihtBadKeys = 0;
  while (1) {
    KeyboardHardware.readMatrix();
    if (KeyboardHardware.leftHandState.all == TEST_MODE_KEY_COMBO) {
      break;
    }
    for (byte row = 0; row < 4; row++) {
      for (byte col = 0; col < 8; col++) {
        uint8_t keynum = (row * 8) + (col);

        uint8_t keyState = ((bitRead(KeyboardHardware.previousLeftHandState.all, keynum) << 1) |
                            (bitRead(KeyboardHardware.leftHandState.all,         keynum) << 0));

        if (keyState == TOGGLED_ON) {
          if (cyclesSinceStateChange[keynum] < CHATTER_CYCLE_LIMIT) {
            bitSet(leftBadKeys, keynum);
          }
          cyclesSinceStateChange[keynum] = 0;
        } else if (cyclesSinceStateChange[keynum] <= CHATTER_CYCLE_LIMIT)  {
          cyclesSinceStateChange[keynum]++;

        }



        // If the key is held down
        if (keyState == HELD) {
          KeyboardHardware.setCrgbAt(row, 7 - col, green);
        }
        // If we triggered chatter detection ever on this key
        else if (bitRead(leftBadKeys, keynum) == 1) {
          KeyboardHardware.setCrgbAt(row, 7 - col, red);
        }

        // If the key was just released
        else if (keyState == TOGGLED_OFF) {
          KeyboardHardware.setCrgbAt(row, 7 - col, blue);
        }


        keyState = ((bitRead(KeyboardHardware.previousRightHandState.all, keynum) << 1) |
                    (bitRead(KeyboardHardware.rightHandState.all,         keynum) << 0));


        if (keyState == TOGGLED_ON) {
          if (cyclesSinceStateChange[keynum + 32] < CHATTER_CYCLE_LIMIT) {
            bitSet(rightBadKeys, keynum);
          }
          cyclesSinceStateChange[keynum + 32] = 0;
        } else if (cyclesSinceStateChange[keynum + 32] <= CHATTER_CYCLE_LIMIT)  {
          cyclesSinceStateChange[keynum + 32]++;
        }

        // If the key is held down
        if (keyState == HELD) {
          KeyboardHardware.setCrgbAt(row, 15 - col, green);
        }
        // If we triggered chatter detection ever on this key
        else if (bitRead(rightBadKeys, keynum) == 1) {
          KeyboardHardware.setCrgbAt(row, 15 - col, red);
        }

        // If the key was just released
        else if (keyState == TOGGLED_OFF) {
          KeyboardHardware.setCrgbAt(row, 15 - col, blue);
        }
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

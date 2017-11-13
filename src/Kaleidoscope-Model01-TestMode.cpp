#include "Kaleidoscope.h"
#include "Kaleidoscope-Model01-TestMode.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"
#include <avr/io.h>
#include <avr/wdt.h>


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
  delay(25);
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


uint32_t olderLeftHandState = 0;
uint32_t olderRightHandState = 0 ;

void TestMode_::testMatrix() {
  LEDControl.set_all_leds_to(200, 0, 0);
  while (1) {
    KeyboardHardware.readMatrix();
    if (KeyboardHardware.leftHandState.all == TEST_MODE_KEY_COMBO) {
      break;
    }
    for (byte row = 0; row < 4; row++) {
      for (byte col = 0; col < 8; col++) {
        uint8_t keynum = (row * 8) + (col);

        uint8_t keyState = (bitRead(olderLeftHandState, keynum) << 2) |
                           (bitRead(KeyboardHardware.previousLeftHandState.all, keynum) << 1) |
                           (bitRead(KeyboardHardware.leftHandState.all,         keynum) << 0);


        // If we had chatter in between two reads
        if (keyState == 2 || keyState == 5) {
          bitSet(leftBadKeys, keynum);
        }


        // If the key is held down
        if (keyState == 7) {
          KeyboardHardware.setCrgbAt(row, 7 - col, green);
        }
        // If we're seeing chatter right now
        else if (keyState == 2 || keyState == 5) {
          KeyboardHardware.setCrgbAt(row, 7 - col, white);
        }

        // If we triggered chatter detection ever on this key
        else if (bitRead(leftBadKeys, keynum)) {
          KeyboardHardware.setCrgbAt(row, 7 - col, red);
        }

        // If the key was just released
        else if (keyState == 6) {
          KeyboardHardware.setCrgbAt(row, 7 - col, blue);
        }


        keyState = (bitRead(olderRightHandState, keynum) << 2) |
                   (bitRead(KeyboardHardware.previousRightHandState.all, keynum) << 1) |
                   (bitRead(KeyboardHardware.rightHandState.all,         keynum) << 0);

        // If we had chatter in between two reads
        if (keyState == 2 || keyState == 5) {
          bitSet(rightBadKeys, keynum);
        }

        // If the key is held down
        if (keyState == 7) {
          KeyboardHardware.setCrgbAt(row, 15 - col, green);
        }
        // If we're seeing chatter right now
        else if (keyState == 2 || keyState == 5) {
          KeyboardHardware.setCrgbAt(row, 15 - col, white);
        }

        // If we triggered chatter detection ever on this key
        else if (bitRead(rightBadKeys, keynum)) {
          KeyboardHardware.setCrgbAt(row, 15 - col, red);
        }

        // If the key was just released
        else if (keyState == 6) {
          KeyboardHardware.setCrgbAt(row, 15 - col, blue);
        }
      }
    }
    LEDControl.syncLeds();
    olderRightHandState = KeyboardHardware.previousRightHandState.all;
    olderLeftHandState = KeyboardHardware.previousLeftHandState.all;
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
  KeyboardHardware.setKeyscanInterval(0);
  test_leds();
  testMatrix();
  //  Serial.println("Done running tests");
}

TestMode_ TestMode;

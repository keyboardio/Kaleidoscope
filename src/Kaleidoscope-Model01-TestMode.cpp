#include "Kaleidoscope.h"
#include "Kaleidoscope-Model01-TestMode.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"
#include <avr/io.h>
#include <avr/wdt.h>


cRGB red;
cRGB blue;
cRGB green;


TestMode_::TestMode_(void) {
}


void TestMode_::begin(void) {
  red.r = 201;
  blue.b = 201;
  green.g = 201;
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

        uint8_t keyState = (bitRead(KeyboardHardware.previousLeftHandState.all, keynum) << 0) |
                           (bitRead(KeyboardHardware.leftHandState.all,         keynum) << 1);

        if (keyState == 3) {
          KeyboardHardware.setCrgbAt(row, 7 - col, green);
        } else if (keyState == 1) {
          KeyboardHardware.setCrgbAt(row, 7 - col, blue);
        }

        keyState = (bitRead(KeyboardHardware.previousRightHandState.all, keynum) << 0) |
                   (bitRead(KeyboardHardware.rightHandState.all,         keynum) << 1);

        if (keyState == 3) {
          KeyboardHardware.setCrgbAt(row, 15 - col, green);
        } else if (keyState == 1) {
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
  KeyboardHardware.setKeyscanInterval(0);
  test_leds();
  testMatrix();
  //  Serial.println("Done running tests");
}

TestMode_ TestMode;

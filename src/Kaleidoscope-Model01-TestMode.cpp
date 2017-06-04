#include "Kaleidoscope.h"
#include "Kaleidoscope-Model01-TestMode.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"

cRGB red;
cRGB blue;
cRGB green;

#define LED_TEST_DELAY 2000

TestMode_::TestMode_(void) {
}


void TestMode_::begin(void) {
  red.r=101;
  blue.b=101;
  green.g=101;
  loop_hook_use(this->loopHook);

}

void TestMode_::loopHook(bool postClear) {
  if (postClear)
    return;
  if (KeyboardHardware.leftHandState.all == (R0C0 | R0C6 | R3C6)
//  && KeyboardHardware.rightHandState.all == combo.rightHand
     ) {
    run_tests();
  }
}

void TestMode_::wait_for_keypress() {
  delay(25);
  while (1) {
    KeyboardHardware.read_matrix();
    if (KeyboardHardware.leftHandState.all ==  R3C6
        && KeyboardHardware.previousLeftHandState.all == 0) {
      break;
    }
  }

}

void TestMode_::set_leds(uint8_t r, uint8_t g, uint8_t b) {
  LEDControl.set_all_leds_to(r,g,b);
  LEDControl.led_sync();
  wait_for_keypress();

}

void TestMode_::test_leds(void) {
  // make all LEDs dim red
  set_leds(50,0,0);
  // make all LEDs dim blue
  set_leds(0,50,0);
  // make all LEDs dim green
  set_leds(0,0,50);
  // make all LEDs dim white
  set_leds(50,50,50);
  // make all the LEDs bright red
  set_leds(200,0,0);
  // make all the LEDs bright green
  set_leds(0,200,0);
  // make all the LEDs bright blue
  set_leds(0,0,200);
  // make all the LEDs bright white (1.6A)
  set_leds(160,160,160);
  // rainbow for 10 seconds
  for (auto i=0; i<1000; i++) {
    LEDRainbowEffect.update();
    LEDControl.led_sync();
  }
  // set all the keys to red
}




void TestMode_::test_matrix() {
  LEDControl.set_all_leds_to(50,0,0);
  while (1) {
    KeyboardHardware.read_matrix();
    if (KeyboardHardware.leftHandState.all == (R0C0 | R0C6 | R3C6)) {
      break;
    }
    for (byte row = 0; row < 4; row++) {
      for (byte col = 0; col < 8; col++) {

        uint8_t keynum = (row*8)+(col);

        uint8_t keyState = (bitRead(KeyboardHardware.previousLeftHandState.all, keynum) << 0) |
                           (bitRead(KeyboardHardware.leftHandState.all,         keynum) << 1);

        if (keyState ==3) {
          Serial.print(" Key: ");
          Serial.print(keynum);
          Serial.print(" value ");
          Serial.println(keyState);
          KeyboardHardware.led_set_crgb_at(row, 7-col, green);
        } else if (keyState ==1) {
          KeyboardHardware.led_set_crgb_at(row,7-col, blue);
        }

        keyState = (bitRead(KeyboardHardware.previousRightHandState.all, keynum) << 0) |
                   (bitRead(KeyboardHardware.rightHandState.all,         keynum) << 1);

        if (keyState ==3) {
          Serial.print(" Key: ");
          Serial.print(keynum);
          Serial.print(" value ");
          Serial.println(keyState);
          KeyboardHardware.led_set_crgb_at(row, 15-col, green);
        } else if (keyState ==1) {
          KeyboardHardware.led_set_crgb_at(row,15-col, blue);
        }
      }
    }
    LEDControl.led_sync();
  }
}

void TestMode_::run_tests() {
  Serial.println("Running tests");
  test_leds();
  test_matrix();
  Serial.println("Done running tests");
}

TestMode_ TestMode;

#include "KeyboardioFirmware.h"
#include "Keyboardio-Model01-TestMode.h"
#include "Keyboardio-LEDEffect-Rainbow.h"

cRGB red;
cRGB blue;

#define LED_TEST_DELAY 2000

uint8_t TestMode_::minInterval = 10;
uint8_t TestMode_::timer;

TestMode_::TestMode_(void) {
}


void TestMode_::begin(void) {
    loop_hook_use (this->loopHook);

}

void TestMode_::loopHook (bool postClear) {
    if (!dictionary || postClear)
      return;
    if (timer && timer < minInterval)
      timer++;
    if (KeyboardHardware.leftHandState.all == R1C3 | R2C1 | R2C4 | R2C7
//  && KeyboardHardware.rightHandState.all == combo.rightHand
	) {
        if (timer == 0 || timer >= minInterval || minInterval == 0) {
          loop();
          timer = 1;
        }
        return;
      }
}

void TestMode_::TestLEDs(void) {
    // make all LEDs dim red
    LEDControl.set_all_leds_to(50,0,0);
    LEDControl.led_sync();
    delay(LED_TEST_DELAY);
    // make all LEDs dim blue
    LEDControl.set_all_leds_to(0,50,0);
    LEDControl.led_sync();
    delay(LED_TEST_DELAY);
    // make all LEDs dim green
    LEDControl.set_all_leds_to(0,0,50);
    LEDControl.led_sync();
    delay(LED_TEST_DELAY);
    // make all LEDs dim white
    LEDControl.set_all_leds_to(50,50,50);
    LEDControl.led_sync();
    delay(LED_TEST_DELAY);
    // make all the LEDs bright red
    LEDControl.set_all_leds_to(200,0,0);
    LEDControl.led_sync();
    delay(LED_TEST_DELAY);
    // make all the LEDs bright green
    LEDControl.set_all_leds_to(0,200,0);
    LEDControl.led_sync();
    delay(LED_TEST_DELAY);
    // make all the LEDs bright blue
    LEDControl.set_all_leds_to(0,0,200);
    LEDControl.led_sync();
    delay(LED_TEST_DELAY);
    // make all the LEDs bright white (1.6A)
    LEDControl.set_all_leds_to(160,160,160);
    LEDControl.led_sync();
    delay(LED_TEST_DELAY);
    // rainbow for 10 seconds
    for(auto i=0; i<1000; i++ ) {
        LEDRainbowEffect.update();
        LEDControl.led_sync();
    }
    // set all the keys to red
    LEDControl.set_all_leds_to(50,0,0);
    // make all LEDs dim blue
    // as you hit each key, set it to blue
    // as you hit each key a second time, set it to green
    // as you hit each key a third time, set it to off
}




void TestMode_::TestMatrix () {
   while(1) {
   	 KeyboardHardware.read_matrix();
    for (byte row = 0; row < 4; row++) {
        for (byte col = 0; col < 8; col++) {

            uint8_t keynum = (row*8)+(col);

            uint8_t keyState = (bitRead(KeyboardHardware.previousLeftHandState.all, keynum) << 0) |
                               (bitRead(KeyboardHardware.leftHandState.all,         keynum) << 1);

		if(keyState ==3 ) {
		Serial.print(" Key: ");
		Serial.print(keynum);
		Serial.print(" value ");
		Serial.println(keyState);
	    		KeyboardHardware.led_set_crgb_at(row, 7-col, red);
		} else if (keyState ==1) {
	    		KeyboardHardware.led_set_crgb_at(row,7-col, blue);
		}

            keyState = (bitRead(KeyboardHardware.previousRightHandState.all, keynum) << 0) |
              	       (bitRead(KeyboardHardware.rightHandState.all,         keynum) << 1);

		if(keyState ==3 ) {
		Serial.print(" Key: ");
		Serial.print(keynum);
		Serial.print(" value ");
		Serial.println(keyState);
	    		KeyboardHardware.led_set_crgb_at(row, 15-col, red);
		} else if (keyState ==1) {
	    		KeyboardHardware.led_set_crgb_at(row,15-col, blue);
		}
        }
    }
	LEDControl.led_sync();
   }
}

void TestMode_::setup() {
red.r=101;
blue.b=101;
	TestLEDs();

}

void TestMode_::loop() {
	TestMatrix();
}

TestMode_ TestMode;

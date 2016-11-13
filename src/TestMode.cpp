#include "KeyboardioFirmware.h"
#include "TestMode.h"

TestMode_::TestMode_(void) {
}

void TestMode_::TestLEDs(void) {
    // make all LEDs dim red
    LEDControl.set_all_leds_to(50,0,0);
    led_sync();
    delay(1000);
    // make all LEDs dim blue
    LEDControl.set_all_leds_to(0,50,0);
    led_sync();
    delay(1000);
    // make all LEDs dim green
    LEDControl.set_all_leds_to(0,0,50);
    led_sync();
    delay(1000);
    // make all LEDs dim white
    LEDControl.set_all_leds_to(50,50,50);
    led_sync();
    delay(1000);
    // make all the LEDs bright red
    LEDControl.set_all_leds_to(200,0,0);
    led_sync();
    delay(1000);
    // make all the LEDs bright green
    LEDControl.set_all_leds_to(0,200,0);
    led_sync();
    delay(1000);
    // make all the LEDs bright blue
    LEDControl.set_all_leds_to(0,0,200);
    led_sync();
    delay(1000);
    // make all the LEDs bright white (1.6A)
    LEDControl.set_all_leds_to(160,160,160);
    led_sync();
    delay(1000);
    // rainbow for 10 seconds
    for(auto i=0; i<10000; i++ ) {
        LEDControl.effect_rainbow_update();
        led_sync();
        delay(1);
    }
    // set all the keys to red
    LEDControl.set_all_leds_to(50,0,0);
    // make all LEDs dim blue
    // as you hit each key, set it to blue
    // as you hit each key a second time, set it to green
    // as you hit each key a third time, set it to off
}



void TestMode_::TestMatrix () {
    KeyboardHardware.read_matrix();

}

void TestMode_::setup() {
    eventHandlers[0] = handle_key_event_test;
}
bool handle_key_event_test(byte row, byte col, uint8_t currentState, uint8_t previousState) {
    Serial.write(row);
}

TestMode_ TestMode;

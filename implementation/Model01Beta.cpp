#include <Arduino.h>
#include "WS2812.h"
#include "Model01Beta.h"


sx1509Class KeyboardHardware_::leftsx1509(LEFT_SX1509_ADDRESS);
sx1509Class KeyboardHardware_::rightsx1509(RIGHT_SX1509_ADDRESS);


WS2812 KeyboardHardware_::LED(LED_COUNT);

KeyboardHardware_::KeyboardHardware_(void) {

}

void KeyboardHardware_::leds_setup() {
    LED.setOutput(LED_DATA_PIN);
    LED.setColorOrderGRB();  // Uncomment for RGB color order
}

void KeyboardHardware_::led_set_crgb_at(byte row, byte col, cRGB color) {
    LED.set_crgb_at(key_led_map[row][col], color);
}

cRGB KeyboardHardware_::get_key_color(byte row, byte col) {
    return LED.get_crgb_at(key_led_map[row][col]);
}

void KeyboardHardware_::led_set_crgb_at(uint8_t i, cRGB crgb) {
    LED.set_crgb_at(i, crgb);
}

void KeyboardHardware_::led_sync() {
    LED.sync();
}

void KeyboardHardware_::scan_row(byte row) {
    if (left_initted) {
        leftsx1509.updatePinState(left_rowpins[row], LOW);
        leftsx1509.sendPinStates();
        leftsx1509.fetchPinStates();
    }
    if (right_initted) {
        rightsx1509.updatePinState(right_rowpins[row], LOW);
        rightsx1509.sendPinStates();
        rightsx1509.fetchPinStates();

    }
}
void KeyboardHardware_::finish_scanning_row(byte row) {
    if (left_initted)
        leftsx1509.updatePinState(left_rowpins[row], HIGH);
    if (right_initted)
        rightsx1509.updatePinState(right_rowpins[row], HIGH);
}

void KeyboardHardware_::scan_left_col(byte row, byte col,uint8_t *state) {

    //If we see an electrical connection on I->J,


    *state <<= 1;

    if (left_initted && leftsx1509.readPrefetchedPin(left_colpins[col])) {
        *state |= 0;
    } else {
        *state |= 1;
    }
}

void KeyboardHardware_::scan_right_col(byte row, byte col, uint8_t *state) {

    //If we see an electrical connection on I->J,

    *state <<= 1;


    if (right_initted && rightsx1509.readPrefetchedPin(right_colpins[col])) {
        *state |= 0;
    } else {
        *state |= 1;
    }

}




boolean KeyboardHardware_::right_hand_connected(void) {
    if (right_initted) {
        return true;
    } else {
        return false;
    }
}

void KeyboardHardware_::pins_setup() {
    right_initted = setup_sx1509(rightsx1509, right_colpins, right_rowpins);
    left_initted = setup_sx1509(leftsx1509, left_colpins, left_rowpins);
    rightsx1509.fetchPinStates();
}


void KeyboardHardware_::make_input(sx1509Class sx1509, uint8_t pin) {
    sx1509.pinDir(pin, INPUT);  // Set SX1509 pin 1 as an input
    sx1509.writePin(pin, HIGH);  // Activate pull-up
}

void KeyboardHardware_::make_output(sx1509Class sx1509, uint8_t pin) {
    sx1509.pinDir(pin, OUTPUT);
    sx1509.writePin(pin, HIGH);
}

int KeyboardHardware_::setup_sx1509 (sx1509Class sx1509, uint8_t colpins[], uint8_t rowpins[]) {
    byte initted;

    for (int counter = 0; counter < 10; counter++) {
        initted = sx1509.init();

        if (initted)
            break;
    }

    if (initted) { // init ok
        // In order to use the keypad, the clock must first be
        // configured. We can call configureClock() with the default
        // parameters (2MHz internal oscillator, no clock in/out).
        sx1509.configClock();


        // the debounceConfig function sets the debounce time. This
        //  function's parameter should be a 3-bit value.
        // 3: 4ms * 2MHz/fOSC
        sx1509.debounceConfig(4);

        for (int i = 0; i < LEFT_ROWS; i++) {
            make_output(sx1509, rowpins[i]);
        }

        for (int j = 0; j < LEFT_COLS; j++) {
            make_input(sx1509, colpins[j]);
            sx1509.debounceEnable(colpins[j]);
        }


    }
    return initted;

}

KeyboardHardware_ KeyboardHardware;

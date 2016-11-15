#include "Model01.h"
KeyboardioScanner Model01::leftHand(0);
KeyboardioScanner Model01::rightHand(3);

Model01::Model01(void) {

}

void Model01::enable_scanner_power(void) {
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
}

// This lets the keyboard pull up to 1.6 amps from
// the host. That violates the USB spec. But it sure
// is pretty looking
void Model01::enable_high_power_leds(void) {
    pinMode(7, OUTPUT);
    digitalWrite(7, LOW);
}

void Model01::setup(void) {
    enable_scanner_power();

    // Consider not doing this until 30s after keyboard
    // boot up, to make it easier to rescue things
    // in case of power draw issues.
    enable_high_power_leds();
    leftHandState.all = 0;
    rightHandState.all = 0;

    Wire.begin();
    TWBR=12; // This is 400mhz, which is the fastest we can drive the ATTiny
}


void Model01::led_set_crgb_at(uint8_t i, cRGB crgb) {
    if(i<32) {
        leftHand.ledData.leds[i] = crgb;
    } else if (i<64) {
        rightHand.ledData.leds[i-32] = crgb;
    } else {
        // TODO how do we want to handle debugging assertions about crazy user
        // code that would overwrite other memory?
    }
}

cRGB Model01::led_get_crgb_at(uint8_t i) {
    if(i<32) {
        return leftHand.ledData.leds[i];
    } else if (i<64) {
        return rightHand.ledData.leds[i-32] ;
    }
}

void Model01::led_sync() {
    leftHand.sendLEDData();
    rightHand.sendLEDData();

    leftHand.sendLEDData();
    rightHand.sendLEDData();

    leftHand.sendLEDData();
    rightHand.sendLEDData();

    leftHand.sendLEDData();
    rightHand.sendLEDData();

}

void debug_key_event(keydata_t state, keydata_t previousState, uint8_t keynum, uint8_t row, uint8_t col) {
    if (bitRead(state.all, keynum) != bitRead(previousState.all, keynum )) {
        Serial.print("Looking at row ");
        Serial.print(row);
        Serial.print(", col ");
        Serial.print(col);
        Serial.print(" key # ");
        Serial.print(keynum);
        Serial.print(" ");
        Serial.print(bitRead(previousState.all, keynum));
        Serial.print(" -> ");
        Serial.print(bitRead(state.all, keynum ));
        Serial.println();
    }
}


void Model01::read_matrix() {
    //scan the Keyboard matrix looking for connections
    previousLeftHandState = leftHandState;
    previousRightHandState = rightHandState;

    if (leftHand.readKeys()) {
        leftHandState = leftHand.getKeyData();
    }

    if (rightHand.readKeys()) {
        rightHandState = rightHand.getKeyData();
    }
}



void Model01::act_on_matrix_scan() {
    for (byte row = 0; row < 4; row++) {
        for (byte col = 0; col < 8; col++) {

            uint8_t keynum = (row*8)+(col);

            handle_key_event(Key_NoKey, row, 7-col,
                             bitRead(leftHandState.all, keynum),
                             bitRead(previousLeftHandState.all, keynum)
                            );

            handle_key_event(Key_NoKey, row, (15- col),
                             bitRead(rightHandState.all, keynum),
                             bitRead(previousRightHandState.all, keynum)
                            );
        }
    }
}


void Model01::scan_matrix() {
    read_matrix();
    act_on_matrix_scan();
}

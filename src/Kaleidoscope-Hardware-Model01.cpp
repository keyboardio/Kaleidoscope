#include <Kaleidoscope.h>
#include <avr/wdt.h>

KeyboardioScanner Model01::leftHand(0);
KeyboardioScanner Model01::rightHand(3);
bool Model01::isLEDChanged = true;

static constexpr uint8_t key_led_map[4][16] = {
  {3,4,11,12,19,20,26,27,     36,37,43,44,51,52,59,60},
  {2,5,10,13,18,21,25,28,     35,38,42,45,50,53,58,61},
  {1,6, 9,14,17,22,24,29,     34,39,41,46,49,54,57,62},
  {0,7, 8,15,16,23,31,30,     33,32,40,47,48,55,56,63},
};

const uint8_t PROGMEM gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255
};

Model01::Model01(void) {

}

void Model01::enable_scanner_power(void) {
    // PC7
    //pinMode(13, OUTPUT);
    //digitalWrite(13, HIGH);
    // Turn on power to the LED net
    DDRC |= _BV(7);
    PORTC |= _BV(7);

}

// This lets the keyboard pull up to 1.6 amps from
// the host. That violates the USB spec. But it sure
// is pretty looking
void Model01::enable_high_power_leds(void) {
    // PE6
    //    pinMode(7, OUTPUT);
    //    digitalWrite(7, LOW);
    DDRE |= _BV(6);
    PORTE &= ~_BV(6);

   // Set B4, the overcurrent check to an input with an internal pull-up
   DDRB &= ~_BV(4);	// set bit, input
   PORTB &= ~_BV(4);	// set bit, enable pull-up resistor



}

void Model01::setup(void) {
    enable_scanner_power();

    // Consider not doing this until 30s after keyboard
    // boot up, to make it easier to rescue things
    // in case of power draw issues.
    enable_high_power_leds();
    leftHandState.all = 0;
    rightHandState.all = 0;

    TWBR=12; // This is 400mhz, which is the fastest we can drive the ATTiny
}


void Model01::led_set_crgb_at(uint8_t i, cRGB crgb_) {
    cRGB crgb;

    crgb.r = pgm_read_byte(&gamma8[crgb_.r]);
    crgb.g = pgm_read_byte(&gamma8[crgb_.g]);
    crgb.b = pgm_read_byte(&gamma8[crgb_.b]);

    if(i<32) {
        cRGB oldColor = led_get_crgb_at(i);
        isLEDChanged |= !(oldColor.r == crgb.r && oldColor.g == crgb.g && oldColor.b == crgb.b);

        leftHand.ledData.leds[i] = crgb;
    } else if (i<64) {
        cRGB oldColor = led_get_crgb_at(i);
        isLEDChanged |= !(oldColor.r == crgb.r && oldColor.g == crgb.g && oldColor.b == crgb.b);

        rightHand.ledData.leds[i-32] = crgb;
    } else {
        // TODO how do we want to handle debugging assertions about crazy user
        // code that would overwrite other memory?
    }
}

void Model01::led_set_crgb_at(byte row, byte col, cRGB color) {
    led_set_crgb_at(key_led_map[row][col], color);
}

cRGB Model01::led_get_crgb_at(uint8_t i) {
    if(i<32) {
        return leftHand.ledData.leds[i];
    } else if (i<64) {
        return rightHand.ledData.leds[i-32] ;
    } else {
        return {0, 0, 0};
    }
}

void Model01::led_sync() {
    if (!isLEDChanged)
        return;

    leftHand.sendLEDData();
    rightHand.sendLEDData();

    leftHand.sendLEDData();
    rightHand.sendLEDData();

    leftHand.sendLEDData();
    rightHand.sendLEDData();

    leftHand.sendLEDData();
    rightHand.sendLEDData();

    isLEDChanged = false;
}

boolean Model01::led_power_fault() {
	if (PINB & _BV(4)) {
		return true;
	} else {
		return false;
	}
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

            uint8_t keyState = (bitRead(previousLeftHandState.all, keynum) << 0) |
              (bitRead(leftHandState.all, keynum) << 1);
            handle_key_event(Key_NoKey, row, 7-col, keyState);

            keyState = (bitRead(previousRightHandState.all, keynum) << 0) |
              (bitRead(rightHandState.all, keynum) << 1);

            handle_key_event(Key_NoKey, row, (15- col), keyState);
        }
    }
}


void Model01::scan_matrix() {
    read_matrix();
    act_on_matrix_scan();
}

void Model01::reboot_bootloader() {
    // Set the magic bits to get a Caterina-based device
    // to reboot into the bootloader and stay there, rather
    // than run move onward
    //
    // These values are the same as those defined in
    // Caterina.c

    uint16_t bootKey = 0x7777;
    uint16_t *const bootKeyPtr = (uint16_t *)0x0800;

    // Stash the magic key
    *bootKeyPtr = bootKey;

    // Set a watchdog timer
    wdt_enable(WDTO_120MS);

    while (1) {} // This infinite loop ensures nothing else
    // happens before the watchdog reboots us
}

HARDWARE_IMPLEMENTATION KeyboardHardware;

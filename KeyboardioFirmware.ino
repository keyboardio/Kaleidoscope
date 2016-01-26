// Copyright 2015 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#define DEBUG_SERIAL false

/**
 *  TODO:

    add mouse inertia
    add series-of-character macros
    add series of keystroke macros
    use a lower-level USB API

*/
#include "KeyboardioFirmware.h"
#include <EEPROM.h>  // Don't need this for CLI compilation, but do need it in the IDE
#include <Wire.h>
#include "KeyboardioSX1509.h"
#include "HID-Project.h"




const byte LEFT_SX1509_ADDRESS = 0x70;  // SX1509 I2C address (10)
const byte RIGHT_SX1509_ADDRESS = 0x71;  // SX1509 I2C address (11)
sx1509Class leftsx1509(LEFT_SX1509_ADDRESS);
sx1509Class rightsx1509(RIGHT_SX1509_ADDRESS);

int right_initted = 0;
int left_initted = 0;



void setup_matrix() {
    //blank out the matrix.
    for (byte col = 0; col < COLS; col++) {
        for (byte row = 0; row < ROWS; row++) {
            matrixState[row][col] = 0;
        }
    }
}


void set_keymap(Key keymapEntry, byte matrixStateEntry) {
    if (keymapEntry.flags & SWITCH_TO_KEYMAP) {
        // this logic sucks. there is a better way TODO this
        if (! (keymapEntry.flags ^ ( MOMENTARY | SWITCH_TO_KEYMAP))) {
            if (key_toggled_on(matrixStateEntry)) {
                if ( keymapEntry.rawKey == KEYMAP_NEXT) {
                    temporary_keymap++;
                } else if ( keymapEntry.rawKey == KEYMAP_PREVIOUS) {
                    temporary_keymap--;
                } else {
                    temporary_keymap = keymapEntry.rawKey;
                }
            }
            if (key_toggled_off(matrixStateEntry)) {

                temporary_keymap = primary_keymap;

            }



        } else if (! (keymapEntry.flags ^ (  SWITCH_TO_KEYMAP))) {
            // switch keymap and stay there
            if (key_toggled_on(matrixStateEntry)) {
                temporary_keymap = primary_keymap = keymapEntry.rawKey;
                save_primary_keymap(primary_keymap);
            }
        }
    }
}

void scan_matrix() {
    x = 0;
    y = 0;
    //scan the Keyboard matrix looking for connections
    for (byte row = 0; row < LEFT_ROWS; row++) {
        TS("Scanning row ")
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


        for (byte col = 0; col < LEFT_COLS; col++) {
            TS("Scanning col")
            //If we see an electrical connection on I->J,

            matrixState[row][col] <<= 1;

            matrixState[row][(COLS - 1) - col] <<= 1;

            TS("Reading left pin")
            if (left_initted && leftsx1509.readPrefetchedPin(left_colpins[col])) {
                matrixState[row][col] |= 0;
            } else {
                matrixState[row][col] |= 1;
            }

            TS("Reading right pin")
            if (right_initted && rightsx1509.readPrefetchedPin(right_colpins[col])) {
                matrixState[row][(COLS - 1) - col] |= 0;
            } else {
                matrixState[row][(COLS - 1) - col] |= 1;
            }


            // while we're inspecting the electrical matrix, we look
            // to see if the Key being held is a firmware level
            // metakey, so we can act on it, lest we only discover
            // that we should be looking at a seconary Keymap halfway
            // through the matrix scan


            TS("calling send_key_event")
            send_key_event(row, col);
            if (right_initted)
                send_key_event(row, (COLS - 1) - col);

        }
        TS("clearing output pins")
        if (left_initted)
            leftsx1509.updatePinState(left_rowpins[row], HIGH);
        if (right_initted)
            rightsx1509.updatePinState(right_rowpins[row], HIGH);
    }
    TS("Sending key report");
    Keyboard.sendReport();
    Keyboard.releaseAll();
    handle_mouse_movement(x, y);
}

// Command mode
//


void setup() {
    wdt_disable();
    Serial.begin(115200);
    //usbMaxPower = 100;
    Keyboard.begin();
    Mouse.begin();
    setup_leds();
    led_bootup();
    setup_matrix();
    setup_pins();
    rightsx1509.fetchPinStates();

    temporary_keymap = primary_keymap = load_primary_keymap();
}

String myApp;

void loop() {
    // if(Serial.available()) {
    // myApp = Serial.readString();
    // myApp.trim();
    // }
    TS("A noop takes...")
    TS("about to scan the matrix")
    scan_matrix();
    TS("updating LEDs");
    update_leds(temporary_keymap == NUMPAD_KEYMAP);
}





void save_primary_keymap(byte keymap) {
    EEPROM.write(EEPROM_KEYMAP_LOCATION, keymap);
}

byte load_primary_keymap() {
    byte keymap =  EEPROM.read(EEPROM_KEYMAP_LOCATION);
    if (keymap >= KEYMAPS ) {
        return 0; // undefined positions get saved as 255
    }
    return 0; //  return keymap;
}





// Sending events to the usb host

void handle_synthetic_key_press(byte switchState, Key mappedKey) {
    if (mappedKey.flags & IS_MOUSE_KEY ) {
        if (mappedKey.rawKey & MOUSE_WARP) {
            if (key_toggled_on(switchState)) {
                warp_mouse(mappedKey);
            }
        } else {
            handle_mouse_key_press(switchState, mappedKey, x, y);
        }
    }
    if (mappedKey.flags & IS_CONSUMER) {
        if (key_toggled_on (switchState)) {
            ConsumerControl.press(mappedKey.rawKey);
        }
    }

    else if (mappedKey.flags & IS_INTERNAL) {
        if (key_toggled_on (switchState)) {
            if (mappedKey.rawKey == LED_TOGGLE) {
                next_led_mode();
            }
        }
    } else if (mappedKey.flags & IS_SYSCTL) {
        if (key_toggled_on (switchState)) {
            SystemControl.press(mappedKey.rawKey);
        }
    } else  if (mappedKey.flags & IS_MACRO) {
        if (key_toggled_on (switchState)) {
            if (mappedKey.rawKey == 1) {
                Serial.print("Keyboard.IO keyboard driver v0.00");
            }
        }
    } else if (mappedKey.rawKey == KEY_MOUSE_BTN_L
               || mappedKey.rawKey == KEY_MOUSE_BTN_M
               || mappedKey.rawKey == KEY_MOUSE_BTN_R) {
        if (key_toggled_on (switchState)) {
            Mouse.press(mappedKey.rawKey);
            end_warping();
        } else if (key_is_pressed(switchState)) {
        } else if (Mouse.isPressed(mappedKey.rawKey) ) {
            Mouse.release(mappedKey.rawKey);
        }
    }
}

void send_key_event(byte row, byte col) {
    //for every newly pressed button, figure out what logical key it is and send a key down event
    // for every newly released button, figure out what logical key it is and send a key up event


    // TODO:switch to sending raw HID packets

    // really, these are signed small ints

    byte switchState = matrixState[row][col];
    Key mappedKey = keymaps[temporary_keymap][row][col];

    set_keymap(keymaps[primary_keymap][row][col], switchState);


    if (mappedKey.flags & SYNTHETIC_KEY) {
        handle_synthetic_key_press(switchState, mappedKey);
    } else {
        if (key_is_pressed(switchState)) {
            press_key(mappedKey);
        }


    }
}

void press_key(Key mappedKey) {
    if (mappedKey.flags & SHIFT_HELD) {
        Keyboard.press(Key_LShift.rawKey);
    }
    Keyboard.press(mappedKey.rawKey);
}

void make_input(sx1509Class sx1509, uint8_t pin) {
    sx1509.pinDir(pin, INPUT);  // Set SX1509 pin 1 as an input
    sx1509.writePin(pin, HIGH);  // Activate pull-up


}

void make_output(sx1509Class sx1509, uint8_t pin) {
    sx1509.pinDir(pin, OUTPUT);
    sx1509.writePin(pin, HIGH);

}


void setup_pins() {
    right_initted = setup_sx1509(rightsx1509, right_colpins, right_rowpins);
    left_initted = setup_sx1509(leftsx1509, left_colpins, left_rowpins);

}


int setup_sx1509 (sx1509Class sx1509, uint8_t colpins[], uint8_t rowpins[]) {
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
        // 0: 0.5ms * 2MHz/fOSC
        // 1: 1ms * 2MHz/fOSC
        // 2: 2ms * 2MHz/fOSC
        // 3: 4ms * 2MHz/fOSC
        // 4: 8ms * 2MHz/fOSC
        // 5: 16ms * 2MHz/fOSC
        // 6: 32ms * 2MHz/fOSC
        // 7: 64ms * 2MHz/fOSC
        sx1509.debounceConfig(4);  // maximum debuonce time


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

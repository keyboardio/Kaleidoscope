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
#include "HID-Project.h"








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
        implementation_scan_row(row);


        for (byte col = 0; col < LEFT_COLS; col++) {
            TS("Scanning col")


            matrixState[row][col] = implementation_scan_left_col(row,col,matrixState[row][col]);
            matrixState[row][(COLS - 1) - col] = implementation_scan_right_col(row,col,matrixState[row][(COLS - 1) - col]);

            // while we're inspecting the electrical matrix, we look
            // to see if the Key being held is a firmware level
            // metakey, so we can act on it, lest we only discover
            // that we should be looking at a seconary Keymap halfway
            // through the matrix scan

            TS("calling send_key_event")
            send_key_event(row, col);

            if (implementation_right_hand_connected()) {
                send_key_event(row, (COLS - 1) - col);
            }
        }
        TS("clearing output pins")
        implementation_finish_scanning_row(row);
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
    implementation_pins_setup();

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



// Copyright 2015 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#define DEBUG_SERIAL false

#include "KeyboardioFirmware.h"
#include "HID-Project.h"

KeyboardStorage Storage;
LEDControl LEDs;



void scan_matrix() {
    //scan the Keyboard matrix looking for connections
    for (byte row = 0; row < LEFT_ROWS; row++) {
        implementation_scan_row(row);

        for (byte col = 0; col < LEFT_COLS; col++) {
            implementation_scan_left_col(row,col,&matrixState[row][col]);
            handle_key_event(row, col);

            if (implementation_right_hand_connected()) {
                implementation_scan_right_col(row,col,&matrixState[row][(COLS - 1) - col]);
                handle_key_event(row, (COLS - 1) - col);
            }
        }
        implementation_finish_scanning_row(row);
    }
}

void setup() {
    wdt_disable();
    Keyboard.begin();
    Mouse.begin();
    implementation_setup_leds();
    LEDs.led_bootup();
    implementation_pins_setup();

    temporary_keymap = primary_keymap = Storage.load_primary_keymap();
}


void loop() {
    scan_matrix();
    Keyboard.sendReport();
    Keyboard.releaseAll();
    LEDs.update_leds(temporary_keymap);
}


// Sending events to the usb host

void handle_synthetic_key_event(byte switchState, Key mappedKey) {
    if (mappedKey.flags & IS_MOUSE_KEY ) {
        if (mappedKey.rawKey & MOUSE_WARP) {
            if (key_toggled_on(switchState)) {
                warp_mouse(mappedKey);
            }
        } else {
            handle_mouse_key_press(switchState, mappedKey);
        }
    }
    if (mappedKey.flags & IS_CONSUMER) {
        if (key_toggled_on (switchState)) {
            ConsumerControl.press(mappedKey.rawKey);
        }
    } else if (mappedKey.flags & IS_INTERNAL) {
        if (key_toggled_on (switchState)) {
            if (mappedKey.rawKey == LED_TOGGLE) {
                LEDs.next_led_mode();
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

void handle_key_event(byte row, byte col) {
    //for every newly pressed button, figure out what logical key it is and send a key down event
    // for every newly released button, figure out what logical key it is and send a key up event

    // really, these are signed small ints

    byte switchState = matrixState[row][col];
    Key mappedKey = keymaps[temporary_keymap][row][col];
    if (keymaps[primary_keymap][row][col].flags & SWITCH_TO_KEYMAP) {
        handle_keymap_key_event(switchState, keymaps[primary_keymap][row][col]);
    }
    if (mappedKey.flags & SYNTHETIC_KEY) {
        handle_synthetic_key_event(switchState, mappedKey);
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


void handle_keymap_key_event(byte switchState, Key keymapEntry) {
    // this logic sucks. there is a better way TODO this
    if (! (keymapEntry.flags ^ ( MOMENTARY | SWITCH_TO_KEYMAP))) {
        if (key_toggled_on(switchState)) {
            if ( keymapEntry.rawKey == KEYMAP_NEXT) {
                temporary_keymap++;
            } else if ( keymapEntry.rawKey == KEYMAP_PREVIOUS) {
                temporary_keymap--;
            } else {
                temporary_keymap = keymapEntry.rawKey;
            }
        }
        if (key_toggled_off(switchState)) {
            temporary_keymap = primary_keymap;
        }

    } else if (! (keymapEntry.flags ^ ( SWITCH_TO_KEYMAP ))) {
        // switch keymap and stay there
        if (key_toggled_on(switchState)) {
            temporary_keymap = primary_keymap = keymapEntry.rawKey;
            Storage.save_primary_keymap(primary_keymap);
        }
    }
}

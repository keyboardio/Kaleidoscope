// Copyright 2015 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#define DEBUG_SERIAL false

#include "KeyboardioFirmware.h"
#include "KeyboardioHID.h"


uint8_t matrixState[ROWS][COLS] = {0};
const Key keymaps[KEYMAPS][ROWS][COLS] = { KEYMAP_LIST };
uint8_t primary_keymap = 0;
uint8_t temporary_keymap = 0;


void scan_matrix() {
    //scan the Keyboard matrix looking for connections
    for (byte row = 0; row < LEFT_ROWS; row++) {
        KeyboardHardware.scan_row(row);

        for (byte col = 0; col < LEFT_COLS; col++) {
            KeyboardHardware.scan_left_col(row,col,&matrixState[row][col]);
            handle_key_event(row, col);

            if (KeyboardHardware.right_hand_connected()) {
                KeyboardHardware.scan_right_col(row,col,&matrixState[row][(COLS - 1) - col]);
                handle_key_event(row, (COLS - 1) - col);
            }
        }
        KeyboardHardware.finish_scanning_row(row);
    }
}

void setup() {
    wdt_disable();
    Keyboard.begin();
    Mouse.begin();
    KeyboardHardware.leds_setup();
    LEDControl.boot_animation();
    KeyboardHardware.pins_setup();

    temporary_keymap = primary_keymap = Storage.load_primary_keymap(KEYMAPS);
}


void loop() {
    scan_matrix();
    Keyboard.sendReport();
    Keyboard.releaseAll();
    LEDControl.update(temporary_keymap);
}



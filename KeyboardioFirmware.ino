// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#define DEBUG_SERIAL false

#include "KeyboardioFirmware.h"
#include "KeyboardioHID.h"


uint8_t primary_keymap = 0;
uint8_t temporary_keymap = 0;

bool handle_user_key_event(byte row, byte col, uint8_t currentState, uint8_t previousState) {
  //Serial.print ("user_key_event");
  return false;
}

void setup() {
    wdt_disable();
    delay(100);
    Keyboard.begin();
    Mouse.begin();
    AbsoluteMouse.begin();
    KeyboardHardware.setup();
    LEDControl.boot_animation();

    temporary_keymap = primary_keymap = Storage.load_primary_keymap(KEYMAPS);
}


void loop() {
    KeyboardHardware.scan_matrix();
    LEDControl.update(temporary_keymap);
    Keyboard.sendReport();
    Keyboard.releaseAll();
}



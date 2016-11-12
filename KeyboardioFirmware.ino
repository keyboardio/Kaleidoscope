// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#define DEBUG_SERIAL false

#include "KeyboardioFirmware.h"

uint8_t primary_keymap = 0;
uint8_t temporary_keymap = 0;

void setup() {
    Keyboardio.setup();
}


void loop() {
    Keyboardio.loop();
}



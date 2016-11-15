// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#define DEBUG_SERIAL false

#include "KeyboardioFirmware.h"
#include "generated/keymaps.h"

uint8_t primary_keymap = 0;
uint8_t temporary_keymap = 0;

#define KEYMAPS 3
#define NUMPAD_KEYMAP 2
#define KEYMAP_LIST KEYMAP_QWERTY KEYMAP_GENERIC_FN2 KEYMAP_NUMPAD

const Key keymaps[][ROWS][COLS] PROGMEM = { KEYMAP_LIST };

void setup() {
    Keyboardio.setup(KEYMAPS, NUMPAD_KEYMAP);
}


void loop() {
    Keyboardio.loop();
}



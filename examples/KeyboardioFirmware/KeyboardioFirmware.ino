// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#define DEBUG_SERIAL false

#include "Keyboardio-MouseKeys.h"
#include "Keyboardio-Macros.h"
#include "Keyboardio-LEDControl.h"
#include "KeyboardioFirmware.h"
#include "generated/keymaps.h"

#include "LED-Off.h"
#include "Keyboardio-LEDEffect-SolidColor.h"
#include "Keyboardio-LEDEffect-Numlock.h"
#include "Keyboardio-LEDEffect-Breathe.h"
#include "Keyboardio-LEDEffect-Chase.h"
#include "Keyboardio-LEDEffect-Rainbow.h"

uint8_t primary_keymap = 0;
uint8_t temporary_keymap = 0;

#define NUMPAD_KEYMAP 2
#define KEYMAP_LIST KEYMAP_QWERTY KEYMAP_GENERIC_FN2 KEYMAP_NUMPAD

const Key keymaps[][ROWS][COLS] PROGMEM = { KEYMAP_LIST };

static LEDSolidColor solidRed (100, 0, 0);
static LEDSolidColor solidOrange (100, 30, 0);
static LEDSolidColor solidYellow (90, 70, 0);
static LEDSolidColor solidGreen (0, 200, 0);
static LEDSolidColor solidBlue (0, 30, 200);
static LEDSolidColor solidIndigo (0, 0, 200);
static LEDSolidColor solidViolet (100, 0, 120);

const macro_t *macroAction(uint8_t macroIndex, byte row, byte col, uint8_t keyState) {
    if (macroIndex == 0 && key_toggled_on(keyState)) {
        return NumLock.toggle (row, col, NUMPAD_KEYMAP);
    }

    if (macroIndex == 1 && key_toggled_on(keyState)) {
        Serial.print("Keyboard.IO keyboard driver v0.00");
        return MACRO(I(25),
                     D(LShift), T(M), U(LShift), T(O), T(D), T(E), T(L),
                     T(Space),
                     W(100),
                     T(0), T(1),
                     END);
    }
    return MACRO_NONE;
}

void setup() {
    Keyboardio.setup(KEYMAP_SIZE);

    Keyboardio.use(&LEDControl, &LEDOff,
                   &solidRed, &solidOrange, &solidYellow, &solidGreen, &solidBlue, &solidIndigo, &solidViolet,
                   &LEDBreatheEffect, &LEDRainbowEffect, &LEDChaseEffect, &NumLock,

                   &Macros,
                   &MouseKeys,
                   NULL);

    LEDOff.activate();
}


void loop() {
    Keyboardio.loop();
}

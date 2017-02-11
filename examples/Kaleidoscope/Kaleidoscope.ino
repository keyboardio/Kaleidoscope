// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#define DEBUG_SERIAL false

#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-Numlock.h"
#include "Kaleidoscope.h"
#include "generated/keymaps.h"

#include "LED-Off.h"
#include "Kaleidoscope-LEDEffect-SolidColor.h"
#include "Kaleidoscope-LEDEffect-Breathe.h"
#include "Kaleidoscope-LEDEffect-Chase.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"
#include "Kaleidoscope-Model01-TestMode.h"

uint8_t primary_keymap = 0;
uint8_t temporary_keymap = 0;

#define NUMPAD_KEYMAP 2
#define KEYMAP_LIST KEYMAP_QWERTY KEYMAP_GENERIC_FN2 KEYMAP_NUMPAD

const Key keymaps[][ROWS][COLS] PROGMEM = { KEYMAP_LIST };

static LEDSolidColor solidRed (60, 0, 0);
static LEDSolidColor solidOrange (60, 20, 0);
static LEDSolidColor solidYellow (40, 35, 0);
static LEDSolidColor solidGreen (0, 100, 0);
static LEDSolidColor solidBlue (0, 15, 100);
static LEDSolidColor solidIndigo (0, 0, 100);
static LEDSolidColor solidViolet (70, 0, 60);

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
    if (macroIndex == TOGGLENUMLOCK && key_toggled_on(keyState)) {
        return NumLock.toggle (Macros.row, Macros.col, NUMPAD_KEYMAP);
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
    Kaleidoscope.setup(KEYMAP_SIZE);

    Kaleidoscope.use(&TestMode,
                     &LEDControl, &LEDOff,
                     &solidRed, &solidOrange, &solidYellow, &solidGreen, &solidBlue, &solidIndigo, &solidViolet,
                     &LEDBreatheEffect, &LEDRainbowEffect, &LEDChaseEffect, &NumLock,

                     &Macros,
                     &MouseKeys,
                     NULL);

    LEDOff.activate();
}


void loop() {
    Kaleidoscope.loop();
}

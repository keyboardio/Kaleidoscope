// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#define DEBUG_SERIAL false

#include "MouseKeys.h"
#include "KeyboardioFirmware.h"
#include "generated/keymaps.h"

#include "LED-BootAnimation.h"
#include "LED-SolidColor.h"
#include "LED-Numlock.h"
#include "LED-BreatheEffect.h"
#include "LED-ChaseEffect.h"
#include "LED-RainbowEffect.h"

uint8_t primary_keymap = 0;
uint8_t temporary_keymap = 0;

#define KEYMAPS 3
#define NUMPAD_KEYMAP 2
#define KEYMAP_LIST KEYMAP_QWERTY KEYMAP_GENERIC_FN2 KEYMAP_NUMPAD

const Key keymaps[][ROWS][COLS] PROGMEM = { KEYMAP_LIST };

static LEDBootAnimation bootAnimation;
static LEDSolidColor solidRed (100, 0, 0);
static LEDSolidColor solidOrange (100, 30, 0);
static LEDSolidColor solidYellow (90, 70, 0);
static LEDSolidColor solidGreen (0, 200, 0);
static LEDSolidColor solidBlue (0, 30, 200);
static LEDSolidColor solidIndigo (0, 0, 200);
static LEDSolidColor solidViolet (100, 0, 120);

static LEDBreatheEffect breatheEffect;
static LEDRainbowEffect rainbowEffect;
static LEDRainbowWaveEffect rainbowWaveEffect;
static LEDChaseEffect chaseEffect;

static LEDNumlock numLockEffect (NUMPAD_KEYMAP);

void setup() {
    Keyboardio.setup(KEYMAPS);
}


void loop() {
    Keyboardio.loop();
}

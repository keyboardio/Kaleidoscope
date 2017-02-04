#pragma once

#include <Arduino.h>

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project KeyboardIO here

#define TS(X) //Serial.print(micros() );Serial.print("\t");Serial.println(X);

#include <stdio.h>
#include <math.h>
#include <avr/wdt.h>

#include KEYBOARDIO_HARDWARE_H
#include "key_events.h"
#include "plugin.h"
#include "layers.h"

extern HARDWARE_IMPLEMENTATION KeyboardHardware;

#ifndef VERSION
#define VERSION "locally-built"
#endif

#define KEYMAP_SIZE (sizeof(keymaps) / ROWS / COLS / sizeof(Key))

class Keyboardio_ {
  public:
    Keyboardio_(void);

    void setup(const byte keymap_count);
    void loop(void);
    void use(KeyboardioPlugin *plugin, ...) __attribute__((sentinel));
};

extern Keyboardio_ Keyboardio;

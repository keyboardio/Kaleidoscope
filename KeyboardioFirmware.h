#pragma once

#include <Arduino.h>
//add your includes for the project KeyboardIO here
#include <EEPROM.h>

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

#include <Wire.h>
#include <stdio.h>
#include <math.h>
#include <avr/wdt.h>

#include "key_defs.h"
#include "KeyboardConfig.h"
#include "led_control.h"
#include "generated/keymaps.h"
#include "debouncing.h"
#include "mouse_movement.h"
#include "storage.h"

uint8_t matrixState[ROWS][COLS] = {0};
static const Key keymaps[KEYMAPS][ROWS][COLS] = { KEYMAP_LIST };

uint8_t primary_keymap = 0;
uint8_t temporary_keymap = 0;

void scan_matrix();

// sending events to the computer
void handle_synthetic_key_event(byte switchState, Key mappedKey);
void handle_key_event(byte row, byte col);
void press_key(Key mappedKey);
void handle_keymap_key_event(byte switchState, Key keymapEntry);
void handle_mouse_key_event(byte switchState, Key mappedKey);

#ifndef VERSION
#define VERSION "locally-built"
#endif


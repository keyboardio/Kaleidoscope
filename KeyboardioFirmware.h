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
#include "KeyboardioSX1509.h"
#include "mouse_movement.h"
//extern uint8_t usbMaxPower;

char x;
char y;

uint8_t matrixState[ROWS][COLS] = {0};
static const Key keymaps[KEYMAPS][ROWS][COLS] = { KEYMAP_LIST };

byte primary_keymap = 0;
byte temporary_keymap = 0;




// EEPROM related
void save_primary_keymap(byte keymap);
byte load_primary_keymap();




// hardware keymap interaction
void setup_pins();
void setup_input_pins();
void setup_output_pins();
void scan_matrix();

// key matrix
void reset_matrix();
void handle_immediate_action_during_matrix_scan(Key keymapEntry, byte matrixStateEntry);

// keymaps
void set_keymap_keymap(Key keymapEntry, byte matrixStateEntry);

// sending events to the computer
void handle_synthetic_key_press(byte switchState, Key mappedKey);
void send_key_events();
void handle_key_event(byte row, byte col);
void press_key(Key mappedKey);


#ifndef VERSION
#define VERSION "locally-built"
#endif


#pragma once
#include <Arduino.h>

#include "key_defs.h"
#include "matrix_state.h"
#include "MouseWrapper.h"
#include "led_control.h"
#include "storage.h"


extern uint8_t matrixState[ROWS][COLS];
extern const Key keymaps[KEYMAPS][ROWS][COLS];
extern uint8_t primary_keymap;
extern uint8_t temporary_keymap;

extern KeyboardStorage Storage;
extern LEDControl LEDs;

// sending events to the computer
void handle_synthetic_key_event(byte switchState, Key mappedKey);
void handle_key_event(byte row, byte col);
void press_key(Key mappedKey);
void handle_keymap_key_event(byte switchState, Key keymapEntry);
void handle_mouse_key_event(byte switchState, Key mappedKey);


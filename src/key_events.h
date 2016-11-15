#pragma once
#include <Arduino.h>

#include "key_defs.h"
#include "keyswitch_state.h"
#include "MouseWrapper.h"
#include "LEDControl.h"
#include "Storage.h"
#include "keymap_metadata.h"
#include "hooks.h"

//static const Key keymaps[KEYMAPS][ROWS][COLS];
extern uint8_t primary_keymap;
extern uint8_t temporary_keymap;
extern const Key keymaps[][ROWS][COLS];

// sending events to the computer
void handle_synthetic_key_event( Key mappedKey, uint8_t currentState, uint8_t previousState);
void handle_key_event(byte row, byte col, uint8_t currentState, uint8_t previousState);
void press_key(Key mappedKey);
void handle_keymap_key_event(Key keymapEntry, uint8_t currentState, uint8_t previousState);
void handle_mouse_key_event(Key mappedKey, uint8_t currentState, uint8_t previousState);
bool handle_key_event_default(byte row, byte col, uint8_t currentState, uint8_t previousState);

Key lookup_key(byte keymap, byte row, byte col);

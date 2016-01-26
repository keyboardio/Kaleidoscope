#pragma once

#include "WS2812.h"
#include "generated/keymaps.h"

#define EEPROM_KEYMAP_LOCATION 0


#define RIGHT_COLS 8
#define RIGHT_ROWS 4
#define LEFT_COLS 8
#define LEFT_ROWS 4

uint8_t left_colpins[]= {7,6,5,4,3,2,1,0};
uint8_t left_rowpins[]= {8,9,10,11};

uint8_t right_colpins[]= {0,1,2,3,4,5,6,7};
uint8_t right_rowpins[]= {8,9,10,11};




#define COLS 16
#define ROWS 4
#define KEYMAPS 3
#define NUMPAD_KEYMAP 2
#define KEYMAP_LIST KEYMAP_QWERTY KEYMAP_GENERIC_FN2 KEYMAP_NUMPAD

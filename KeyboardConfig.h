#pragma once

#include "WS2812.h"
#include "generated/keymaps.h"

#define EEPROM_KEYMAP_LOCATION 0


int RIGHT_COLS=8;
int RIGHT_ROWS=4;
int LEFT_COLS=8;
int LEFT_ROWS=4;
int left_colpins[]= {7,6,5,4,3,2,1,0};
int left_rowpins[]= {8,9,10,11};

int right_colpins[]= {0,1,2,3,4,5,6,7};
int right_rowpins[]= {8,9,10,11};




#define COLS 16
#define ROWS 4
#define KEYMAPS 3
#define NUMPAD_KEYMAP 2
#define KEYMAP_LIST KEYMAP_QWERTY KEYMAP_GENERIC_FN2 KEYMAP_NUMPAD

#include <Arduino.h>
#include <EEPROM.h>
#include "KeyboardConfig.h"

void save_primary_keymap(byte keymap);
byte load_primary_keymap();

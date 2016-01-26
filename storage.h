#include <Arduino.h>
#include <EEPROM.h>
#include "KeyboardConfig.h"

class KeyboardStorage {
    public:
        byte load_primary_keymap();
        void save_primary_keymap(byte keymap);
};

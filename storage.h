#include <Arduino.h>
#include <EEPROM.h>
#include "KeyboardConfig.h"

class KeyboardStorage {
    public:
        uint8_t load_primary_keymap();
        void save_primary_keymap(uint8_t keymap);
};

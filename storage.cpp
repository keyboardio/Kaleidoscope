#include "storage.h"



void KeyboardStorage::save_primary_keymap(uint8_t keymap) {
    EEPROM.write(EEPROM_KEYMAP_LOCATION, keymap);
}

uint8_t KeyboardStorage::load_primary_keymap() {
    uint8_t keymap =  EEPROM.read(EEPROM_KEYMAP_LOCATION);
    if (keymap >= KEYMAPS ) {
        return 0; // undefined positions get saved as 255
    }
    return 0; //  return keymap;
}


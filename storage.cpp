#include "storage.h"

void save_primary_keymap(byte keymap) {
    EEPROM.write(EEPROM_KEYMAP_LOCATION, keymap);
}

byte load_primary_keymap() {
    byte keymap =  EEPROM.read(EEPROM_KEYMAP_LOCATION);
    if (keymap >= KEYMAPS ) {
        return 0; // undefined positions get saved as 255
    }
    return 0; //  return keymap;
}


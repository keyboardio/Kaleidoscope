#include "storage.h"


Storage_::Storage_(void) {
}

void Storage_::save_primary_keymap(uint8_t keymap) {
    EEPROM.write(EEPROM_KEYMAP_LOCATION, keymap);
}

uint8_t Storage_::load_primary_keymap() {
    uint8_t keymap =  EEPROM.read(EEPROM_KEYMAP_LOCATION);
    if (keymap >= KEYMAPS ) {
        return 0; // undefined positions get saved as 255
    }
    return 0; //  return keymap;
}

Storage_ Storage;

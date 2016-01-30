#pragma once

#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_KEYMAP_LOCATION 0

class Storage_ {
  public:
    Storage_(void);
    uint8_t load_primary_keymap(uint8_t keymap_count);
    void save_primary_keymap(uint8_t keymap);
};

extern Storage_ Storage;

#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include "KeyboardConfig.h"

class Storage_ {
  public:
    Storage_(void);
    uint8_t load_primary_keymap(void);
    void save_primary_keymap(uint8_t keymap);
};

extern Storage_ Storage;

// -*- mode: c++ -*-

#include "Kaleidoscope.h"
#include "Kaleidoscope-FocusSerial.h"
#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"
#include "Kaleidoscope-MouseKeys.h"

KEYMAPS(
    [0] = KEYMAP(Key_A, Key_B, Key_C, Key_D, Key_E, Key_NoKey),
);

KALEIDOSCOPE_INIT_PLUGINS(
    Focus,
    EEPROMSettings,
    EEPROMKeymap,
    MouseKeys
);

void setup() {
  Kaleidoscope.setup();
  EEPROMKeymap.setup(5);
}

void loop() {
  Kaleidoscope.loop();
}

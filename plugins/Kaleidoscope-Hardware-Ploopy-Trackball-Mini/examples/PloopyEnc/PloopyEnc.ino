// -*- mode: c++ -*-

#include "Kaleidoscope.h"
#include "Kaleidoscope-FocusSerial.h"
#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-Macros.h"

KEYMAPS(
    [0] = KEYMAP(M(0), Key_B, Key_C, Key_D, Key_E, Key_NoKey),
);

#define OPT_ENC1 PIN_F0
#define OPT_ENC2 PIN_F4
#define OPT_ENC1_MUX 0
#define OPT_ENC2_MUX 4

void handleEncoder() {
  uint16_t p1 = analogRead(OPT_ENC1_MUX);
  uint16_t p2 = analogRead(OPT_ENC2_MUX);

  if (p1 != 0 || p2 != 0) {
    Focus.send("p1=", p1, "p2=", p2, "\n");
  }
}

const macro_t *macroAction(uint8_t macro_id, KeyEvent &event) {
  if (!keyToggledOn(event.state)) {
    return MACRO_NONE;
  }

  switch (macro_id) {
    case 0:
      handleEncoder();
      break;
  }

  return MACRO_NONE;
}

KALEIDOSCOPE_INIT_PLUGINS(
    Focus,
    EEPROMSettings,
    EEPROMKeymap,
    MouseKeys,
    Macros
);

void setup() {
  Kaleidoscope.setup();
  EEPROMKeymap.setup(5);

  DDR_INPUT(OPT_ENC1);
  ENABLE_PULLUP(OPT_ENC1);
  DDR_INPUT(OPT_ENC2);
  ENABLE_PULLUP(OPT_ENC2);
}

void loop() {
  Kaleidoscope.loop();

  handleEncoder();
  delay(10);
}

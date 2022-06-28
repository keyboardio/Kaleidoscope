// -*- mode: c++ -*-

#include "Kaleidoscope.h"
#include "Kaleidoscope-FocusSerial.h"
#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-Macros.h"

#include "kaleidoscope/driver/sensor/ADNS5050.h"

struct PTBMProps : kaleidoscope::driver::sensor::ADNS5050BaseProps {
  static constexpr uint8_t clock_pin = PIN_B7;
  static constexpr uint8_t io_pin = PIN_C6;
  static constexpr uint8_t chip_select_pin = PIN_B4;
};

typedef kaleidoscope::driver::sensor::ADNS5050<PTBMProps> SensorT;
SensorT Sensor;

KEYMAPS(
    [0] = KEYMAP(M(0), M(1), M(2), Key_D, Key_E, Key_NoKey),
);

void deltaX() {
  uint8_t dx = Sensor.readRegister(Sensor.Register::DELTA_X);
  Serial.print("dx = ");
  Serial.println(dx);
}

void deltaY() {
  uint8_t dy = Sensor.readRegister(Sensor.Register::DELTA_Y);
  Serial.print("dy = ");
  Serial.println(dy);
}

void getPID() {
  uint8_t pid = Sensor.readRegister(Sensor.Register::PRODUCT_ID);
  Serial.print("pid = ");
  Serial.println(pid);
}

const macro_t *macroAction(uint8_t macro_id, KeyEvent &event) {
  if (!keyToggledOn(event.state)) {
    return MACRO_NONE;
  }

  switch (macro_id) {
    case 0:
      deltaX();
      break;
    case 1:
      deltaY();
      break;
    case 2:
      getPID();
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

  Sensor.setup();
}

void loop() {
  SensorT::Report report;

  Kaleidoscope.loop();

  report = Sensor.readReport();

  if (report.dx != -1) {
    Serial.print("dx = ");
    Serial.print(report.dx);
    Serial.print(" ");
  }
  if (report.dy != -1) {
    Serial.print("dy = ");
    Serial.print(report.dy);
  }
  if (report.dy != -1 || report.dx != -1) {
    Serial.println();
  }
}

// -*- mode: c++ -*-

#include "Kaleidoscope.h"
#include "Kaleidoscope-FocusSerial.h"

#include "kaleidoscope/device/avr/pins_and_ports.h"  // IWYU pragma: keep
#include <avr/wdt.h>

extern "C" {
#include "adns5050.h"
}

KALEIDOSCOPE_INIT_PLUGINS(Focus);

void setup() {
  Kaleidoscope.setup();
  Serial.begin(9600);

  adns5050_init();
  adns5050_sync();

  while (!Serial);

  uint8_t pid = adns5050_read_reg(0x00);
  uint8_t rid = adns5050_read_reg(0x01);
  uint8_t pid2 = adns5050_read_reg(0x3e);

  Serial.print("pid=");
  Serial.print(pid, HEX);
  Serial.print("; rid=");
  Serial.print(rid, HEX);
  Serial.print("; pid2=");
  Serial.println(pid2);
}

void handleSerial() {
  if (Serial.available() == 0) return;

  uint8_t b = Serial.read();

  if (b == 'r') Kaleidoscope.device().rebootBootloader();

  Serial.flush();
}

void loop() {
  handleSerial();

  report_adns5050_t r = adns5050_read_burst();
  if (r.dx != 0 || r.dy != 0) {
    Focus.send("dx=", r.dx, "dy=", r.dy, "\n");
  }

  //delay(250);
}

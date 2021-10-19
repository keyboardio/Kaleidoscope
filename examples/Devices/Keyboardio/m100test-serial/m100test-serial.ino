// -*- mode: c++ -*-

#include <Arduino.h>
#include <Kaleidoscope.h>
#include <Kaleidoscope-Macros.h>

KALEIDOSCOPE_INIT_PLUGINS(Macros);

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  auto s = Kaleidoscope.device().serialPort();

  if (s.available()) {
    uint8_t r = s.read();

    if (r == '1') {
      s.println("Hello");
      Kaleidoscope.device().hid().keyboard().pressKey(Key_H);
      Kaleidoscope.device().hid().keyboard().sendReport();
      Kaleidoscope.device().hid().keyboard().releaseKey(Key_H);
      Kaleidoscope.device().hid().keyboard().sendReport();
    }
  }
}

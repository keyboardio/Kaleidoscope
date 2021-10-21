// -*- mode: c++ -*-

#include <Arduino.h>
#include <Kaleidoscope.h>
#include <Kaleidoscope-Macros.h>

KALEIDOSCOPE_INIT_PLUGINS(Macros);

void setup() {
  Kaleidoscope.setup();
}

void loop() {
      Kaleidoscope.device().hid().keyboard().pressKey(Key_H);
      Kaleidoscope.device().hid().keyboard().sendReport();
      Kaleidoscope.device().hid().keyboard().releaseKey(Key_H);
      Kaleidoscope.device().hid().keyboard().sendReport();
delay(1000);

}

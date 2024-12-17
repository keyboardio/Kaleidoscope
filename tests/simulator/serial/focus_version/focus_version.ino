// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#include "Kaleidoscope.h"
#include "Kaleidoscope-FocusSerial.h"

#define KALEIDOSCOPE_FIRMWARE_VERSION "0.1.2"

#include "Kaleidoscope-FirmwareVersion.h"
// *INDENT-OFF*
// *INDENT-OFF*

KEYMAPS(
  [0] = KEYMAP_STACKED
  (
    XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX

   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
          ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX   ,XXX   ,XXX   ,XXX
   ,XXX
  )
) // KEYMAPS(

// *INDENT-ON*


KALEIDOSCOPE_INIT_PLUGINS(Focus, FirmwareVersion);

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}
/*
Copyright (c) 2014-2015 NicoHood
Copyright (c) 2015-2018 Keyboard.io, Inc

See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// Include guard
#pragma once

#include <Arduino.h>
#include "../HID.h"
#include "../HID-Settings.h"

#include "../HIDTables.h"
#include "../HIDAliases.h"

#define KEY_BYTES 28

typedef union {
  // Modifiers + keymap
  struct {
    uint8_t modifiers;
    uint8_t keys[KEY_BYTES ];
  };
  uint8_t allkeys[1 + KEY_BYTES];
} HID_KeyboardReport_Data_t;

class Keyboard_ {
 public:
  Keyboard_(void);
  void begin(void);
  void end(void);

  size_t press(uint8_t k);
  size_t release(uint8_t k);
  void  releaseAll(void);
  int sendReport(void);

  boolean isKeyPressed(uint8_t k);
  boolean wasKeyPressed(uint8_t k);
  boolean isModifierActive(uint8_t k);
  boolean wasModifierActive(uint8_t k);
  boolean isAnyModifierActive();
  boolean wasAnyModifierActive();

  uint8_t getLEDs() {
    return HID().getLEDs();
  }

  HID_KeyboardReport_Data_t keyReport;
  HID_KeyboardReport_Data_t lastKeyReport;
 private:
  int sendReportUnchecked(void);
};
extern Keyboard_ Keyboard;

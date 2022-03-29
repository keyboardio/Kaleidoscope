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
#include "kaleidoscope/driver/hid/keyboardio/usb/HID_.h"
#include "kaleidoscope/driver/hid/keyboardio/usb/HID-Settings.h"
#include "kaleidoscope/driver/hid/keyboardio/usb/HIDAliases.h"
#include "kaleidoscope/HIDTables.h"

typedef union {
  // Low level key report: up to 6 keys and shift, ctrl etc at once
  struct {
    uint8_t modifiers;
    uint8_t reserved;
    uint8_t keycodes[6];
  };
  uint8_t bytes[8];
} HID_BootKeyboardReport_Data_t;


class BootKeyboard_ : public PluggableUSBModule {
 public:
  BootKeyboard_(uint8_t protocol_ = HID_REPORT_PROTOCOL);
  size_t press(uint8_t k);
  void begin();
  void end();
  size_t release(uint8_t k);
  void releaseAll();

  int sendReport();

  bool isModifierActive(uint8_t k);
  bool wasModifierActive(uint8_t k);
  bool isAnyModifierActive();
  bool wasAnyModifierActive();
  bool isKeyPressed(uint8_t k);
  bool wasKeyPressed(uint8_t k);

  uint8_t getLeds();
  uint8_t getProtocol();
  void setProtocol(uint8_t protocol);

  uint8_t default_protocol;

 protected:
  HID_BootKeyboardReport_Data_t report_, last_report_;

  // Implementation of the PUSBListNode
  int getInterface(uint8_t* interfaceCount);
  int getDescriptor(USBSetup& setup);
  bool setup(USBSetup& setup);

  EPTYPE_DESCRIPTOR_SIZE epType[1];
  uint8_t protocol;
  uint8_t idle;

  uint8_t leds;
};
extern BootKeyboard_ BootKeyboard;

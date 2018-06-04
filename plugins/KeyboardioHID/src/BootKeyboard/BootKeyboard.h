/*
Copyright (c) 2014-2015 NicoHood
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
#include "PluggableUSB.h"
#include "HID.h"
#include "HID-Settings.h"
#include "HIDTables.h"
#include "HIDAliases.h"

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
  BootKeyboard_(void);
  size_t press(uint8_t);
  void begin(void);
  void end(void);
  size_t release(uint8_t);
  void releaseAll(void);

  int sendReport(void);

  boolean isModifierActive(uint8_t k);
  boolean wasModifierActive(uint8_t k);

  uint8_t getLeds(void);
  uint8_t getProtocol(void);
  void setProtocol(uint8_t protocol);

  /* The USB specification says that keyboards should start with
   * HID_REPORT_PROTOCOL as a default, and yet, we default to HID_BOOT_PROTOCOL
   * instead. We do this to support operating systems that require the keyboard
   * to default to boot protocol during negotiation in order to enable them to
   * wake the host up. This includes Linux, MacOS, and Windows. If the keyboard
   * conforms to the spec, and starts in report mode, it will work under these
   * operating systems, but if they are suspended, the keyboard will not be
   * allowed to wake them up.
   *
   * Furthermore, there are BIOSes out there that do not implement a HID
   * descriptor parser, nor do they explicitly send a SET_PROTOCOL request to
   * put the keyboard into boot protocol. For these to work, we need to default
   * to boot protocol.
   *
   * We could - and for some time, did - work the first issue around by having
   * another end-point that implements a boot-keyboard, with boot-protocol as
   * default. Sadly, that has side-effects too: some BIOSes, Grub, and OSX's
   * FileVault gets easily confused, and will use the first node of a device. If
   * that happens to be the dummy wakeup-only keyboard, then the keyboard will
   * be unusable.
   *
   * There is no reliable way to support all these scenarios while still
   * conforming to the specification.
   *
   * Luckily for us, all three major OSes are fine with boot being the default,
   * they will all explicitly set the protocol to report. Any spec-conforming
   * host will also work fine with boot being the default, because they are
   * required - by spec - to set the protocol themselves.
   */
  uint8_t default_protocol = HID_BOOT_PROTOCOL;

 protected:
  HID_BootKeyboardReport_Data_t _keyReport, _lastKeyReport;

  // Implementation of the PUSBListNode
  int getInterface(uint8_t* interfaceCount);
  int getDescriptor(USBSetup& setup);
  bool setup(USBSetup& setup);

  uint8_t epType[1];
  uint8_t protocol;
  uint8_t idle;

  uint8_t leds;
};
extern BootKeyboard_ BootKeyboard;

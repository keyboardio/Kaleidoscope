/*
  Copyright (c) 2015, Arduino LLC
  Copyright (c) 2024 Keyboard.io, Inc
  Original code (pre-library): Copyright (c) 2011, Peter Barrett

  Permission to use, copy, modify, and/or distribute this software for
  any purpose with or without fee is hereby granted, provided that the
  above copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
  WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
  BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
  OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
  SOFTWARE.
 */

#pragma once

#include <stdint.h>
#include <Arduino.h>
#include "kaleidoscope/driver/hid/HIDDefs.h"
#include "HIDD.h"
#include "HID-Settings.h"

#if defined(USBCON)

#define _USING_HID

#pragma pack(push, 1)
typedef struct {
  uint8_t len;       // 9
  uint8_t dtype;     // 0x21
  uint8_t versionL;  // 0x101
  uint8_t versionH;  // 0x101
  uint8_t country;
  uint8_t numDescs;
  uint8_t desctype;  // 0x22 report
  uint8_t descLenL;
  uint8_t descLenH;
} HIDDescDescriptor;

typedef struct {
  InterfaceDescriptor hid;
  HIDDescDescriptor desc;
  EndpointDescriptor in;
} HIDDescriptor;
#pragma pack(pop)

class HIDSubDescriptor {
 public:
  HIDSubDescriptor *next = NULL;
  HIDSubDescriptor(const void *d, const uint16_t l)
    : data(d), length(l) {}

  const void *data;
  const uint16_t length;
};

class HID_ : public HIDD {
 public:
  HID_();
  int begin();
  int SendReport(uint8_t id, const void *data, int len) override;
  void AppendDescriptor(HIDSubDescriptor *node);
  uint8_t getLEDs() {
    return outReport[1];
  }

 protected:
  // Implementation of the PluggableUSBModule
  int getDescriptor(USBSetup &setup) override;
  uint8_t getShortName(char *name);

 private:
  HIDSubDescriptor *rootNode;
};

// Replacement for global singleton.
// This function prevents static-initialization-order-fiasco
// https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
HID_ &HID();

#define D_HIDREPORT(length) \
  { 9, 0x21, 0x11, 0x01, 0, 1, 0x22, lowByte(length), highByte(length) }

#endif  // USBCON

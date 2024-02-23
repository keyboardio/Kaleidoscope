/*
Copyright (c) 2014-2015 NicoHood
Copyright (c) 2015-2024 Keyboard.io, Inc

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

/* HID driver back end */

#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "HIDArch.h" /* unfortunately needed for EPTYPE_DESCRIPTOR_SIZE */
#include "kaleidoscope/driver/hid/HIDDefs.h"

/* Maximum length of output report sent by host */
#define HIDD_MAX_OUTPUT_REPORT 2

class HIDD : public PluggableUSBModule {
 public:
  HIDD(
    const void *_reportDesc = NULL,
    uint16_t descriptorSize = 0,
    uint8_t _inReportLen    = USB_EP_SIZE,
    uint8_t _itfProtocol    = HID_ITF_PROTOCOL_NONE,
    uint8_t _interval       = 1,
    uint8_t _idle           = 0);
  virtual int SendReport(uint8_t id, const void *data, int len);
  uint8_t getProtocol() {
    return protocol;
  }

 protected:
  // Implementation of the PluggableUSBModule
  int getInterface(uint8_t *interfaceCount);
  int getDescriptor(USBSetup &setup);
  bool setup(USBSetup &setup);

  void plug() {
    PluggableUSB().plug(this);
  }

  const void *reportDesc;
  uint16_t descriptorSize;
  uint8_t inReportLen;
  /* Interface protocol, for interface descriptor (only boot devices) */
  uint8_t itfProtocol;
  uint8_t interval;
  uint8_t idle;
  uint8_t outReportLen;

  /* Current HID protocol: Boot or Report */
  uint8_t protocol;

  uint8_t outReport[HIDD_MAX_OUTPUT_REPORT];
  virtual void setReportCB(const void *data, uint16_t len) {
    (void)data;
    (void)len;
  }

 private:
  EPTYPE_DESCRIPTOR_SIZE epType[1];
  int SendReportNoID(const void *data, int len);
};

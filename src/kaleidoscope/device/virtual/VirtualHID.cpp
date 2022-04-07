/*
   Copyright (c) 2015, Arduino LLC
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

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD

// This is a modified version of the original HID.cpp from
// library KeyboardioHID. It replaces all hardware related stuff
// with stub implementations.

#include <stddef.h>  // for NULL
#include <stdint.h>  // for uint8_t

// From KeyboardioHID:
#include "HID.h"                // for HID_, HIDSubDescriptor, HID_::(anonymous), HID, HID_REPOR...
#include "HIDReportObserver.h"  // for HIDReportObserver
#include "PluggableUSB.h"       // for USBSetup, EP_TYPE_INTERRUPT_IN, PluggableUSBModule

#if defined(USBCON)

HID_ &HID() {
  static HID_ obj;
  return obj;
}

int HID_::getInterface(uint8_t *interfaceCount) {
  *interfaceCount += 1;  // uses 1
  return 0;
}

int HID_::getDescriptor(USBSetup &setup) {
  return 1;
}

void HID_::AppendDescriptor(HIDSubDescriptor *node) {
  if (!rootNode) {
    rootNode = node;
  } else {
    HIDSubDescriptor *current = rootNode;

    while (current->next) {
      current = current->next;
    }

    current->next = node;
  }

  descriptorSize += node->length;
}

int HID_::SendReport(uint8_t id, const void *data, int len) {
  HIDReportObserver::observeReport(id, data, len, 0);
  return 1;
}

bool HID_::setup(USBSetup &setup) {
  return true;
}

HID_::HID_(void)
  : PluggableUSBModule(1, 1, epType),
    rootNode(NULL),
    descriptorSize(0),
    protocol(HID_REPORT_PROTOCOL),
    idle(1) {
  setReportData.reportId = 0;
  setReportData.leds     = 0;
  epType[0]              = EP_TYPE_INTERRUPT_IN;
  //PluggableUSB().plug(this);
}

int HID_::begin(void) {
  return 0;
}

#endif /* if defined(USBCON) */

#endif  // #ifdef KALEIDOSCOPE_VIRTUAL_BUILD

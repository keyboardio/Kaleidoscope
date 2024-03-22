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

#ifndef KEYBOARDIOHID_BUILD_WITHOUT_HID

#include "HID.h"
#include "HIDReportObserver.h"
#include "BootKeyboard/BootKeyboard.h"

#if defined(USBCON)

HID_ &HID() {
  static HID_ obj;
  return obj;
}

/* Override to send all report descriptors */
int HID_::getDescriptor(USBSetup &setup) {
  // Check if this is a HID Class Descriptor request
  if (setup.bmRequestType != REQUEST_DEVICETOHOST_STANDARD_INTERFACE) {
    return 0;
  }
  if (setup.wValueH != HID_DESC_TYPE_REPORT) {
    return 0;
  }

  // In a HID Class Descriptor wIndex cointains the interface number
  if (setup.wIndex != pluggedInterface) {
    return 0;
  }

  int total = 0;
  HIDSubDescriptor *node;
  USB_PackMessages(true);
  for (node = rootNode; node; node = node->next) {
    int res = USB_SendControl(TRANSFER_PGM, node->data, node->length);
    if (res == -1)
      return -1;
    total += res;
  }

  USB_PackMessages(false);
  return total;
}

__attribute__((weak))
uint8_t
HID_::getShortName(char *name) {
  name[0] = 'k';
  name[1] = 'b';
  name[2] = 'i';
  name[3] = 'o';
  name[4] = '0';
  name[5] = '1';
  return 6;
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
  auto result = HIDD::SendReport(id, data, len);
  HIDReportObserver::observeReport(id, data, len, result);
  return result;
}

HID_::HID_()
  : rootNode(NULL) {
  // Invoke BootKeyboard constructor so it will be the first HID interface
  (void)BootKeyboard();
  inReportLen = USB_EP_SIZE;
  plug();
}

int HID_::begin() {
  return 0;
}

#endif /* if defined(USBCON) */

#endif /* ifndef KEYBOARDIOHID_BUILD_WITHOUT_HID */

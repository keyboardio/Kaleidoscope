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

#include "SingleAbsoluteMouse.h"
#include "BootKeyboard/BootKeyboard.h"
#include "HIDReportObserver.h"
#include "HID-Settings.h"

static const uint8_t _hidSingleReportDescriptorAbsoluteMouse[] PROGMEM = {
  D_USAGE_PAGE, D_PAGE_GENERIC_DESKTOP, /* USAGE_PAGE (Generic Desktop)         54 */
  D_USAGE,
  D_USAGE_MOUSE, /* USAGE (Mouse) */
  D_COLLECTION,
  D_APPLICATION, /* COLLECTION (Application) */
  D_USAGE,
  D_USAGE_POINTER, /* USAGE (Pointer) */
  D_COLLECTION,
  D_PHYSICAL, /* COLLECTION (Physical)*/

  DESCRIPTOR_ABS_MOUSE_BUTTONS DESCRIPTOR_ABS_MOUSE_XY DESCRIPTOR_ABS_MOUSE_WHEEL D_END_COLLECTION,
  D_END_COLLECTION /* End */
};

#ifdef ARCH_HAS_CONFIGURABLE_EP_SIZES
static const uint8_t SINGLE_ABSOLUTEMOUSE_EP_SIZE = 6;
#else
static const uint8_t SINGLE_ABSOLUTEMOUSE_EP_SIZE = USB_EP_SIZE;
#endif


SingleAbsoluteMouse_::SingleAbsoluteMouse_()
  : PluggableUSBModule(1, 1, epType), protocol(HID_REPORT_PROTOCOL), idle(1) {

  // Invoke BootKeyboard constructor so it will be the first HID interface
  (void)BootKeyboard();
#ifdef ARCH_HAS_CONFIGURABLE_EP_SIZES
  epType[0] = EP_TYPE_INTERRUPT_IN(SINGLE_ABSOLUTEMOUSE_EP_SIZE);
#else
  epType[0] = EP_TYPE_INTERRUPT_IN;
#endif

  PluggableUSB().plug(this);
}

int SingleAbsoluteMouse_::getInterface(uint8_t *interfaceCount) {
  *interfaceCount += 1;  // uses 1
  HIDDescriptor hidInterface = {
    D_INTERFACE(pluggedInterface, 1, USB_DEVICE_CLASS_HUMAN_INTERFACE, HID_SUBCLASS_NONE, HID_PROTOCOL_NONE),
    D_HIDREPORT(sizeof(_hidSingleReportDescriptorAbsoluteMouse)),
    D_ENDPOINT(USB_ENDPOINT_IN(pluggedEndpoint), USB_ENDPOINT_TYPE_INTERRUPT, SINGLE_ABSOLUTEMOUSE_EP_SIZE, 0x01)};
  return USB_SendControl(0, &hidInterface, sizeof(hidInterface));
}

int SingleAbsoluteMouse_::getDescriptor(USBSetup &setup) {
  // Check if this is a HID Class Descriptor request
  if (setup.bmRequestType != REQUEST_DEVICETOHOST_STANDARD_INTERFACE) {
    return 0;
  }
  if (setup.wValueH != HID_REPORT_DESCRIPTOR_TYPE) {
    return 0;
  }

  // In a HID Class Descriptor wIndex cointains the interface number
  if (setup.wIndex != pluggedInterface) {
    return 0;
  }

  return USB_SendControl(TRANSFER_PGM, _hidSingleReportDescriptorAbsoluteMouse, sizeof(_hidSingleReportDescriptorAbsoluteMouse));
}

bool SingleAbsoluteMouse_::setup(USBSetup &setup) {
  if (pluggedInterface != setup.wIndex) {
    return false;
  }

  uint8_t request     = setup.bRequest;
  uint8_t requestType = setup.bmRequestType;

  if (requestType == REQUEST_DEVICETOHOST_CLASS_INTERFACE) {
    if (request == HID_GET_REPORT) {
      // TODO(anyone): HID_GetReport();
      return true;
    }
    if (request == HID_GET_PROTOCOL) {
      return false;
    }
  }

  if (requestType == REQUEST_HOSTTODEVICE_CLASS_INTERFACE) {
    if (request == HID_SET_PROTOCOL) {
      return false;
    }
    if (request == HID_SET_IDLE) {
      idle = setup.wValueL;
      return true;
    }
    if (request == HID_SET_REPORT) {
    }
  }

  return false;
}

void SingleAbsoluteMouse_::sendReport(void *data, int length) {
  auto result = USB_Send(pluggedEndpoint | TRANSFER_RELEASE, data, length);
  HIDReportObserver::observeReport(HID_REPORTID_MOUSE_ABSOLUTE, data, length, result);
}

SingleAbsoluteMouse_ SingleAbsoluteMouse;

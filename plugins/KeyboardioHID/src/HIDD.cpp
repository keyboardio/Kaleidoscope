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

#include "HID.h"
#include "HIDD.h"
#include "HID-Settings.h"

HIDD::HIDD(
  const void *_reportDesc,
  uint16_t _descriptorSize,
  uint8_t _inReportLen,
  uint8_t _itfProtocol,
  uint8_t _interval,
  uint8_t _idle)
  : PluggableUSBModule(1, 1, epType) {

  reportDesc     = _reportDesc;
  descriptorSize = _descriptorSize;
  inReportLen    = _inReportLen;
  itfProtocol    = _itfProtocol;
  interval       = _interval;
  idle           = _idle;
  protocol       = HID_PROTOCOL_REPORT;

#ifdef ARCH_HAS_CONFIGURABLE_EP_SIZES
  epType[0] = EP_TYPE_INTERRUPT_IN(inReportLen);
#else
  epType[0] = EP_TYPE_INTERRUPT_IN;
#endif
}

/* PluggableUSB implementation: interface, etc. descriptors for config */
int HIDD::getInterface(uint8_t *interfaceCount) {
  uint8_t itfSubClass;

  if (itfProtocol != HID_ITF_PROTOCOL_NONE) {
    itfSubClass = HID_SUBCLASS_BOOT;
  } else {
    itfSubClass = HID_SUBCLASS_NONE;
  }

  HIDDescriptor descSet = {
    D_INTERFACE(pluggedInterface, 1, USB_DEVICE_CLASS_HUMAN_INTERFACE, itfSubClass, itfProtocol),
    D_HIDREPORT(descriptorSize),
    D_ENDPOINT(USB_ENDPOINT_IN(pluggedEndpoint), USB_ENDPOINT_TYPE_INTERRUPT, inReportLen, interval),
  };
  *interfaceCount += 1;
  return USB_SendControl(0, &descSet, sizeof(descSet));
}

/* PluggableUSB implementation: class descriptors */
int HIDD::getDescriptor(USBSetup &setup) {
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

  return USB_SendControl(TRANSFER_PGM, reportDesc, descriptorSize);
}

/* PluggableUSB implementation: control requests */
bool HIDD::setup(USBSetup &setup) {
  if (pluggedInterface != setup.wIndex) {
    return false;
  }

  uint8_t request     = setup.bRequest;
  uint8_t requestType = setup.bmRequestType;

  if (requestType == REQUEST_DEVICETOHOST_CLASS_INTERFACE) {
    if (request == HID_REQ_CONTROL_GET_REPORT) {
      // TODO(anyone): HID_GetReport();
      return true;
    }
    if (request == HID_REQ_CONTROL_GET_PROTOCOL) {
      if (itfProtocol == HID_ITF_PROTOCOL_NONE) {
        return false;
      }
      // AVR optimization; possibly not needed elsewhere
#if defined(__AVR__)
      UEDATX = protocol;
#elif defined(ARDUINO_ARCH_SAM)
      USBDevice.armSend(0, &protocol, 1);
#else
      USB_SendControl(TRANSFER_RELEASE, &protocol, sizeof(protocol));
#endif
      return true;
    }
    if (request == HID_REQ_CONTROL_GET_IDLE) {
      // AVR optimization; possibly not needed elsewhere
#if defined(__AVR__)
      UEDATX = idle;
#elif defined(ARDUINO_ARCH_SAM)
      USBDevice.armSend(0, &idle, 1);
#else
      USB_SendControl(TRANSFER_RELEASE, &idle, sizeof(idle));
#endif
      return true;
    }
  }

  if (requestType == REQUEST_HOSTTODEVICE_CLASS_INTERFACE) {
    if (request == HID_REQ_CONTROL_SET_PROTOCOL) {
      if (itfProtocol == HID_ITF_PROTOCOL_NONE) {
        return false;
      }
      protocol = setup.wValueL;
      return true;
    }
    if (request == HID_REQ_CONTROL_SET_IDLE) {
      idle = setup.wValueL;
      return true;
    }
    if (request == HID_REQ_CONTROL_SET_REPORT) {
      // Check if data has the correct length afterwards
      int length = setup.wLength;

      if (setup.wValueH == HID_REPORT_TYPE_OUTPUT) {
        if (length <= sizeof(outReport)) {
          USB_RecvControl(outReport, length);
          setReportCB(outReport, length);
          return true;
        }
      }
    }
  }

  return false;
}

int HIDD::SendReportNoID(const void *data, int len) {
  return USB_Send(pluggedEndpoint | TRANSFER_RELEASE, data, len);
}

int HIDD::SendReport(uint8_t id, const void *data, int len) {
  if (id == 0) {
    return SendReportNoID(data, len);
  }
  /* On SAMD, we need to send the whole report in one batch; sending the id, and
   * the report itself separately does not work, the report never arrives. Due
   * to this, we merge the two into a single buffer, and send that.
   *
   * While the same would work for other architectures, AVR included, doing so
   * costs RAM, which is something scarce on AVR. So on that platform, we opt to
   * send the id and the report separately instead. */
#ifdef ARDUINO_ARCH_SAMD
  uint8_t p[64];
  p[0] = id;
  memcpy(&p[1], data, len);
  return USB_Send(pluggedEndpoint, p, len + 1);
#else
  auto ret = USB_Send(pluggedEndpoint, &id, 1);
  if (ret < 0) return ret;
  auto ret2 = USB_Send(pluggedEndpoint | TRANSFER_RELEASE, data, len);
  if (ret2 < 0) return ret2;
  return ret + ret2;
#endif
}

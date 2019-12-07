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

#ifndef KEYBOARDIOHID_BUILD_WITHOUT_HID

#include "HID.h"
#include "HIDReportObserver.h"

#if defined(USBCON)

HID_& HID() {
    static HID_ obj;
    return obj;
}

int HID_::getInterface(uint8_t* interfaceCount) {
    *interfaceCount += 1; // uses 1
    HIDDescriptor hidInterface = {
        D_INTERFACE(pluggedInterface, 1, USB_DEVICE_CLASS_HUMAN_INTERFACE, HID_SUBCLASS_NONE, HID_PROTOCOL_NONE),
        D_HIDREPORT(descriptorSize),
        D_ENDPOINT(USB_ENDPOINT_IN(pluggedEndpoint), USB_ENDPOINT_TYPE_INTERRUPT, USB_EP_SIZE, 0x01)
    };
    return USB_SendControl(0, &hidInterface, sizeof(hidInterface));
}

int HID_::getDescriptor(USBSetup& setup) {
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

    int total = 0;
    HIDSubDescriptor* node;
    USB_PackMessages(true);
    for (node = rootNode; node; node = node->next) {
        int res = USB_SendControl(TRANSFER_PGM, node->data, node->length);
        if (res == -1)
            return -1;
        total += res;
    }

    // Reset the protocol on reenumeration. Normally the host should not assume the state of the protocol
    // due to the USB specs, but Windows and Linux just assumes its in report mode.
    protocol = HID_REPORT_PROTOCOL;

    USB_PackMessages(false);
    return total;
}

__attribute__((weak))
uint8_t HID_::getShortName(char *name) {
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

int HID_::SendReport(uint8_t id, const void* data, int len) {
    auto result = SendReport_(id, data, len);
    HIDReportObserver::observeReport(id, data, len, result);
    return result;
}

int HID_::SendReport_(uint8_t id, const void* data, int len) {
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
    return USB_Send(pluggedEndpoint, p, len+1);
#else
    auto ret = USB_Send(pluggedEndpoint, &id, 1);
    if (ret < 0) return ret;
    auto ret2 = USB_Send(pluggedEndpoint | TRANSFER_RELEASE, data, len);
    if (ret2 < 0) return ret2;
    return ret + ret2;
#endif
}

bool HID_::setup(USBSetup& setup) {
    if (pluggedInterface != setup.wIndex) {
        return false;
    }

    uint8_t request = setup.bRequest;
    uint8_t requestType = setup.bmRequestType;

    if (requestType == REQUEST_DEVICETOHOST_CLASS_INTERFACE) {
        if (request == HID_GET_REPORT) {
            // TODO: HID_GetReport();
            return true;
        }
        if (request == HID_GET_PROTOCOL) {
            // TODO: Send8(protocol);
            return true;
        }
        if (request == HID_GET_IDLE) {
            // TODO: Send8(idle);
        }
    }

    if (requestType == REQUEST_HOSTTODEVICE_CLASS_INTERFACE) {
        if (request == HID_SET_PROTOCOL) {
            // The USB Host tells us if we are in boot or report mode.
            // This only works with a real boot compatible device.
            protocol = setup.wValueL;
            return true;
        }
        if (request == HID_SET_IDLE) {
            idle = setup.wValueL;
            return true;
        }
        if (request == HID_SET_REPORT) {
            uint16_t length = setup.wLength;

            if (length == sizeof(setReportData)) {
                USB_RecvControl(&setReportData, length);
            } else if (length == sizeof(setReportData.leds)) {
                USB_RecvControl(&setReportData.leds, length);
                setReportData.reportId = 0;
            }
        }
    }

    return false;
}

HID_::HID_(void) : PluggableUSBModule(1, 1, epType),
    rootNode(NULL), descriptorSize(0),
    protocol(HID_REPORT_PROTOCOL), idle(1) {
    setReportData.reportId = 0;
    setReportData.leds = 0;
    epType[0] = EP_TYPE_INTERRUPT_IN;
    PluggableUSB().plug(this);
}

int HID_::begin(void) {
    return 0;
}

#endif /* if defined(USBCON) */

#endif /* ifndef KEYBOARDIOHID_BUILD_WITHOUT_HID */

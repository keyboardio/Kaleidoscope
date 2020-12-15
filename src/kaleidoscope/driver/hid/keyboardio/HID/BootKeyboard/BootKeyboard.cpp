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

#include "BootKeyboard.h"
#include "../DescriptorPrimitives.h"
#include "../HIDReportObserver.h"

// See Appendix B of USB HID spec
static const uint8_t _hidReportDescriptorKeyboard[] PROGMEM = {
    //  Keyboard
    D_USAGE_PAGE, D_PAGE_GENERIC_DESKTOP,
    D_USAGE, D_USAGE_KEYBOARD,

    D_COLLECTION, D_APPLICATION,
    // Modifiers
    D_USAGE_PAGE, D_PAGE_KEYBOARD,
    D_USAGE_MINIMUM, 0xe0,
    D_USAGE_MAXIMUM, 0xe7,
    D_LOGICAL_MINIMUM, 0x0,
    D_LOGICAL_MAXIMUM, 0x1,
    D_REPORT_SIZE, 0x1,
    D_REPORT_COUNT, 0x8,
    D_INPUT, (D_DATA|D_VARIABLE|D_ABSOLUTE),

    // Reserved byte
    D_REPORT_COUNT, 0x1,
    D_REPORT_SIZE, 0x8,
    D_INPUT, (D_CONSTANT),

    // LEDs
    D_REPORT_COUNT, 0x5,
    D_REPORT_SIZE, 0x1,
    D_USAGE_PAGE, D_PAGE_LEDS,
    D_USAGE_MINIMUM, 0x1,
    D_USAGE_MAXIMUM, 0x5,
    D_OUTPUT, (D_DATA|D_VARIABLE|D_ABSOLUTE),
    // Pad LEDs up to a byte
    D_REPORT_COUNT, 0x1,
    D_REPORT_SIZE, 0x3,
    D_OUTPUT, (D_CONSTANT),

    // Non-modifiers
    D_REPORT_COUNT, 0x6,
    D_REPORT_SIZE, 0x8,
    D_LOGICAL_MINIMUM, 0x0,
    D_LOGICAL_MAXIMUM, 0xff,
    D_USAGE_PAGE, D_PAGE_KEYBOARD,
    D_USAGE_MINIMUM, 0x0,
    D_USAGE_MAXIMUM, 0xff,
    D_INPUT, (D_DATA|D_ARRAY|D_ABSOLUTE),
    D_END_COLLECTION
};

BootKeyboard_::BootKeyboard_(void) : PluggableUSBModule(1, 1, epType), protocol(HID_REPORT_PROTOCOL), idle(1), leds(0) {
    epType[0] = EP_TYPE_INTERRUPT_IN;
}

int BootKeyboard_::getInterface(uint8_t* interfaceCount) {
    *interfaceCount += 1; // uses 1
    HIDDescriptor hidInterface = {
        D_INTERFACE(pluggedInterface, 1, USB_DEVICE_CLASS_HUMAN_INTERFACE, HID_SUBCLASS_BOOT_INTERFACE, HID_PROTOCOL_KEYBOARD),
        D_HIDREPORT(sizeof(_hidReportDescriptorKeyboard)),
        D_ENDPOINT(USB_ENDPOINT_IN(pluggedEndpoint), USB_ENDPOINT_TYPE_INTERRUPT, USB_EP_SIZE, 0x01)
    };
    return USB_SendControl(0, &hidInterface, sizeof(hidInterface));
}

int BootKeyboard_::getDescriptor(USBSetup& setup) {
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

    // Reset the protocol on reenumeration. Normally the host should not assume the state of the protocol
    // due to the USB specs, but Windows and Linux just assumes its in report mode.
    protocol = default_protocol;

    return USB_SendControl(TRANSFER_PGM, _hidReportDescriptorKeyboard, sizeof(_hidReportDescriptorKeyboard));
}


void BootKeyboard_::begin(void) {
    PluggableUSB().plug(this);

    // Force API to send a clean report.
    // This is important for and HID bridge where the receiver stays on,
    // while the sender is resetted.
    releaseAll();
    sendReport();
}


void BootKeyboard_::end(void) {
    releaseAll();
    sendReport();
}



bool BootKeyboard_::setup(USBSetup& setup) {
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
            // TODO improve
#ifdef __AVR__
            UEDATX = protocol;
#endif
#ifdef ARDUINO_ARCH_SAM
            USBDevice.armSend(0, &protocol, 1);
#endif
            return true;
        }
        if (request == HID_GET_IDLE) {
            // TODO improve
#ifdef __AVR__
            UEDATX = idle;
#endif
#ifdef ARDUINO_ARCH_SAM
            USBDevice.armSend(0, &idle, 1);
#endif
            return true;
        }
    }

    if (requestType == REQUEST_HOSTTODEVICE_CLASS_INTERFACE) {
        if (request == HID_SET_PROTOCOL) {
            protocol = setup.wValueL;
            return true;
        }
        if (request == HID_SET_IDLE) {
            // We currently ignore SET_IDLE, because we don't really do anything with it, and implementing
            // it causes issues on OSX, such as key chatter. Other operating systems do not suffer if we
            // force this to zero, either.
#if 0
            idle = setup.wValueL;
#else
            idle = 0;
#endif
            return true;
        }
        if (request == HID_SET_REPORT) {
            // Check if data has the correct length afterwards
            int length = setup.wLength;

            if (setup.wValueH == HID_REPORT_TYPE_OUTPUT) {
                if (length == sizeof(leds)) {
                    USB_RecvControl(&leds, length);
                    return true;
                }
            }

            // Input (set HID report)
            else if (setup.wValueH == HID_REPORT_TYPE_INPUT) {
                if (length == sizeof(_keyReport)) {
                    USB_RecvControl(&_keyReport, length);
                    return true;
                }
            }
        }
    }

    return false;
}

uint8_t BootKeyboard_::getLeds(void) {
    return leds;
}

uint8_t BootKeyboard_::getProtocol(void) {
    return protocol;
}

void BootKeyboard_::setProtocol(uint8_t protocol) {
    this->protocol = protocol;
}

int BootKeyboard_::sendReport(void) {
    if (memcmp(&_lastKeyReport, &_keyReport, sizeof(_keyReport))) {
        // if the two reports are different, send a report
        int returnCode = USB_Send(pluggedEndpoint | TRANSFER_RELEASE, &_keyReport, sizeof(_keyReport));
        HIDReportObserver::observeReport(HID_REPORTID_KEYBOARD, &_keyReport, sizeof(_keyReport), returnCode);
        memcpy(&_lastKeyReport, &_keyReport, sizeof(_keyReport));
        return returnCode;
    }
    return -1;
}

// press() adds the specified key (printing, non-printing, or modifier)
// to the persistent key report and sends the report.  Because of the way
// USB HID works, the host acts like the key remains pressed until we
// call release(), releaseAll(), or otherwise clear the report and resend.


size_t BootKeyboard_::press(uint8_t k) {
    uint8_t done = 0;

    if ((k >= HID_KEYBOARD_FIRST_MODIFIER) && (k <= HID_KEYBOARD_LAST_MODIFIER)) {
        // it's a modifier key
        _keyReport.modifiers |= (0x01 << (k - HID_KEYBOARD_FIRST_MODIFIER));
    } else {
        // it's some other key:
        // Add k to the key report only if it's not already present
        // and if there is an empty slot.
        for (uint8_t i = 0; i < sizeof(_keyReport.keycodes); i++) {
            if (_keyReport.keycodes[i] != k) { // is k already in list?
                if (0 == _keyReport.keycodes[i]) { // have we found an empty slot?
                    _keyReport.keycodes[i] = k;
                    done = 1;
                    break;
                }
            } else {
                done = 1;
                break;
            }
        }
        // use separate variable to check if slot was found
        // for style reasons - we do not know how the compiler
        // handles the for() index when it leaves the loop
        if (0 == done) {
            return 0;
        }
    }
    return 1;
}


// release() takes the specified key out of the persistent key report and
// sends the report.  This tells the OS the key is no longer pressed and that
// it shouldn't be repeated any more.

size_t BootKeyboard_::release(uint8_t k) {
    if ((k >= HID_KEYBOARD_FIRST_MODIFIER) && (k <= HID_KEYBOARD_LAST_MODIFIER)) {
        // it's a modifier key
        _keyReport.modifiers = _keyReport.modifiers & (~(0x01 << (k - HID_KEYBOARD_FIRST_MODIFIER)));
    } else {
        // it's some other key:
        // Test the key report to see if k is present.  Clear it if it exists.
        // Check all positions in case the key is present more than once (which it shouldn't be)
        for (uint8_t i = 0; i < sizeof(_keyReport.keycodes); i++) {
            if (_keyReport.keycodes[i] == k) {
                _keyReport.keycodes[i] = 0;
            }
        }

        // rearrange the keys list so that the free (= 0x00) are at the
        // end of the keys list - some implementations stop for keys at the
        // first occurence of an 0x00 in the keys list
        // so (0x00)(0x01)(0x00)(0x03)(0x02)(0x00) becomes
        //    (0x03)(0x02)(0x01)(0x00)(0x00)(0x00)
        uint8_t current = 0, nextpos = 0;

        while (current < sizeof(_keyReport.keycodes)) {
            if (_keyReport.keycodes[current]) {
                uint8_t tmp = _keyReport.keycodes[nextpos];
                _keyReport.keycodes[nextpos] = _keyReport.keycodes[current];
                _keyReport.keycodes[current] = tmp;
                ++nextpos;
            }
            ++current;
        }
    }

    return 1;
}


void BootKeyboard_::releaseAll(void) {
    memset(&_keyReport.bytes, 0x00, sizeof(_keyReport.bytes));
}


/* Returns true if the non-modifer key passed in will be sent during this key report
 * Returns false in all other cases
 * */
boolean BootKeyboard_::isKeyPressed(uint8_t k) {
    for (uint8_t i = 0; i < sizeof(_keyReport.keycodes); i++) {
        if (_keyReport.keycodes[i] == k) {
            return true;
        }
    }
    return false;
}

/* Returns true if the non-modifer key passed in was sent during the previous key report
 * Returns false in all other cases
 * */
boolean BootKeyboard_::wasKeyPressed(uint8_t k) {
    for (uint8_t i = 0; i < sizeof(_keyReport.keycodes); i++) {
        if (_lastKeyReport.keycodes[i] == k) {
            return true;
        }
    }
    return false;
}



/* Returns true if the modifer key passed in will be sent during this key report
 * Returns false in all other cases
 * */
boolean BootKeyboard_::isModifierActive(uint8_t k) {
    if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
        k = k - HID_KEYBOARD_FIRST_MODIFIER;
        return !!(_keyReport.modifiers & (1 << k));
    }
    return false;
}

/* Returns true if the modifer key passed in was being sent during the previous key report
 * Returns false in all other cases
 * */
boolean BootKeyboard_::wasModifierActive(uint8_t k) {
    if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
        k = k - HID_KEYBOARD_FIRST_MODIFIER;
        return !!(_lastKeyReport.modifiers & (1 << k));
    }
    return false;
}

/* Returns true if any modifier key will be sent during this key report
 * Returns false in all other cases
 * */
boolean BootKeyboard_::isAnyModifierActive() {
    return _keyReport.modifiers > 0;
}

/* Returns true if any modifier key was being sent during the previous key report
 * Returns false in all other cases
 * */
boolean BootKeyboard_::wasAnyModifierActive() {
    return _lastKeyReport.modifiers > 0;
}

BootKeyboard_ BootKeyboard;

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

#include "BootKeyboard.h"

static const uint8_t _hidReportDescriptorKeyboard[] PROGMEM = {
  //  Keyboard
  0x05, 0x01,                      /* USAGE_PAGE (Generic Desktop)	  47 */
  0x09, 0x06,                      /* USAGE (Keyboard) */
  0xa1, 0x01,                      /* COLLECTION (Application) */
  0x05, 0x07,                      /*   USAGE_PAGE (Keyboard) */

  /* Keyboard Modifiers (shift, alt, ...) */
  0x19, 0xe0,                      /*   USAGE_MINIMUM (Keyboard LeftControl) */
  0x29, 0xe7,                      /*   USAGE_MAXIMUM (Keyboard Right GUI) */
  0x15, 0x00,                      /*   LOGICAL_MINIMUM (0) */
  0x25, 0x01,                      /*   LOGICAL_MAXIMUM (1) */
  0x75, 0x01,                      /*   REPORT_SIZE (1) */
  0x95, 0x08,                      /*   REPORT_COUNT (8) */
  0x81, 0x02,                      /*   INPUT (Data,Var,Abs) */

  /* 5 LEDs for num lock etc, 3 left for advanced, custom usage */
  0x05, 0x08,						 /*   USAGE_PAGE (LEDs) */
  0x19, 0x01,						 /*   USAGE_MINIMUM (Num Lock) */
  0x29, 0x08,						 /*   USAGE_MAXIMUM (Kana + 3 custom)*/
  0x95, 0x08,						 /*   REPORT_COUNT (8) */
  0x75, 0x01,						 /*   REPORT_SIZE (1) */
  0x91, 0x02,						 /*   OUTPUT (Data,Var,Abs) */

  /* 6 Keyboard keys */
  0x05, 0x07,                      /*   USAGE_PAGE (Keyboard) */
  0x95, 0x06,                      /*   REPORT_COUNT (6) */
  0x75, 0x08,                      /*   REPORT_SIZE (8) */
  0x15, 0x00,                      /*   LOGICAL_MINIMUM (0) */
  0x26, 0xE7, 0x00,                /*   LOGICAL_MAXIMUM (231) */
  0x19, 0x00,                      /*   USAGE_MINIMUM (Reserved (no event indicated)) */
  0x29, 0xE7,                      /*   USAGE_MAXIMUM (Keyboard Right GUI) */
  0x81, 0x00,                      /*   INPUT (Data,Ary,Abs) */

  /* End */
  0xc0                            /* END_COLLECTION */
};

BootKeyboard_::BootKeyboard_(void) : PluggableUSBModule(1, 1, epType), protocol(HID_REPORT_PROTOCOL), idle(1), leds(0) {
  epType[0] = EP_TYPE_INTERRUPT_IN;
  PluggableUSB().plug(this);
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
      UEDATX = protocol;
      return true;
    }
    if (request == HID_GET_IDLE) {
      // TODO improve
      UEDATX = idle;
      return true;
    }
  }

  if (requestType == REQUEST_HOSTTODEVICE_CLASS_INTERFACE) {
    if (request == HID_SET_PROTOCOL) {
      protocol = setup.wValueL;
      return true;
    }
    if (request == HID_SET_IDLE) {
      idle = setup.wValueL;
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
    _keyReport.modifiers = _keyReport.modifiers & (~(0x01 << (k - HID_KEYBOARD_LAST_MODIFIER)));
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
  memset(&_keyReport.keys, 0x00, sizeof(_keyReport.keys));
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

BootKeyboard_ BootKeyboard;

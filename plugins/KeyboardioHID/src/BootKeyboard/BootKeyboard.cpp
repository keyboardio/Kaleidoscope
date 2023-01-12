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
#include "DescriptorPrimitives.h"
#include "HIDReportObserver.h"
#include "HID-Settings.h"

// See Appendix B of USB HID spec
static const uint8_t boot_keyboard_hid_descriptor_[] PROGMEM = {
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
  D_INPUT, (D_DATA | D_VARIABLE | D_ABSOLUTE),

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
  D_OUTPUT, (D_DATA | D_VARIABLE | D_ABSOLUTE),
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
  D_INPUT, (D_DATA | D_ARRAY | D_ABSOLUTE),
  D_END_COLLECTION
};

#ifdef ARCH_HAS_CONFIGURABLE_EP_SIZES
static const uint8_t BOOT_KEYBOARD_EP_SIZE = 8;
#else
static const uint8_t BOOT_KEYBOARD_EP_SIZE = USB_EP_SIZE;
#endif


BootKeyboard_::BootKeyboard_(uint8_t protocol_) : PluggableUSBModule(1, 1, epType), default_protocol(protocol_), protocol(protocol_), idle(0), leds(0) {
#ifdef ARCH_HAS_CONFIGURABLE_EP_SIZES
  epType[0] = EP_TYPE_INTERRUPT_IN(BOOT_KEYBOARD_EP_SIZE); // This is an 8 byte report, so ask for an 8 byte buffer, so reports aren't split
#else
  epType[0] = EP_TYPE_INTERRUPT_IN;
#endif
  PluggableUSB().plug(this);
}

int BootKeyboard_::getInterface(uint8_t* interfaceCount) {
  *interfaceCount += 1; // uses 1
  HIDDescriptor hidInterface = {
    D_INTERFACE(pluggedInterface, 1, USB_DEVICE_CLASS_HUMAN_INTERFACE, HID_SUBCLASS_BOOT_INTERFACE, HID_PROTOCOL_KEYBOARD),
    D_HIDREPORT(sizeof(boot_keyboard_hid_descriptor_)),
    D_ENDPOINT(USB_ENDPOINT_IN(pluggedEndpoint), USB_ENDPOINT_TYPE_INTERRUPT, BOOT_KEYBOARD_EP_SIZE, 0x01)
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

  return USB_SendControl(TRANSFER_PGM, boot_keyboard_hid_descriptor_, sizeof(boot_keyboard_hid_descriptor_));
}


void BootKeyboard_::begin() {
}


void BootKeyboard_::end() {
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
#if defined(__AVR__)
      UEDATX = protocol;
#elif defined(ARDUINO_ARCH_SAM)
      USBDevice.armSend(0, &protocol, 1);
#else
      USB_SendControl(TRANSFER_RELEASE, &protocol, sizeof(protocol));
#endif
      return true;
    }
    if (request == HID_GET_IDLE) {
      // TODO improve
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
      

      // ------------------------------------------------------------
      // Workaround for a bug in the GD32 core:
      //
      // On GD32, when we call `USV_RecvControl`, it casts the (void*) pointer
      // we give it to `uint16_t*`, which means that it will alway write an even
      // number of bytes to that pointer.  Because we don't want to overwrite
      // the next byte in memory past `leds`, we use a temporary array that is
      // guaranteed to be big enough, and copy the data from that:
      if (setup.wValueH == HID_REPORT_TYPE_OUTPUT) {
        if (length == sizeof(leds)) {
          uint8_t raw_report_data[2];
          USB_RecvControl(&raw_report_data, length);
          leds = raw_report_data[0];
          return true;
//        } else {
//	  char tmp[8];
//	  USB_RecvControl(&tmp, length);
//	  return true;
	}	
      }
      // Once the GD32 core bug is fixed, we can replace the above code with the
      // original code below:
      // ------------------------------------------------------------
      // if (setup.wValueH == HID_REPORT_TYPE_OUTPUT) {
      //   if (length == sizeof(leds)) {
      //     USB_RecvControl(&leds, length);
      //     return true;
      //   }
      // }
      // ------------------------------------------------------------

      // Input (set HID report)
      else if (setup.wValueH == HID_REPORT_TYPE_INPUT) {
        if (length == sizeof(report_)) {
          USB_RecvControl(&report_, length);
          return true;
        }
      }
    }
  }

  return false;
}

uint8_t BootKeyboard_::getLeds() {
  return leds;
}

uint8_t BootKeyboard_::getProtocol() {
  return protocol;
}

void BootKeyboard_::setProtocol(uint8_t protocol) {
  this->protocol = protocol;
}

int BootKeyboard_::sendReport() {
  if (memcmp(&last_report_, &report_, sizeof(report_))) {
    // if the two reports are different, send a report
    int returnCode = USB_Send(pluggedEndpoint | TRANSFER_RELEASE, &report_, sizeof(report_));
    HIDReportObserver::observeReport(HID_REPORTID_KEYBOARD, &report_, sizeof(report_), returnCode);
    memcpy(&last_report_, &report_, sizeof(report_));
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
    report_.modifiers |= (0x01 << (k - HID_KEYBOARD_FIRST_MODIFIER));
  } else {
    // it's some other key:
    // Add k to the key report only if it's not already present
    // and if there is an empty slot.
    for (uint8_t i = 0; i < sizeof(report_.keycodes); i++) {
      if (report_.keycodes[i] != k) { // is k already in list?
        if (0 == report_.keycodes[i]) { // have we found an empty slot?
          report_.keycodes[i] = k;
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
    report_.modifiers = report_.modifiers & (~(0x01 << (k - HID_KEYBOARD_FIRST_MODIFIER)));
  } else {
    // it's some other key:
    // Test the key report to see if k is present.  Clear it if it exists.
    // Check all positions in case the key is present more than once (which it shouldn't be)
    for (uint8_t i = 0; i < sizeof(report_.keycodes); i++) {
      if (report_.keycodes[i] == k) {
        report_.keycodes[i] = 0;
      }
    }

    // rearrange the keys list so that the free (= 0x00) are at the
    // end of the keys list - some implementations stop for keys at the
    // first occurence of an 0x00 in the keys list
    // so (0x00)(0x01)(0x00)(0x03)(0x02)(0x00) becomes
    //    (0x03)(0x02)(0x01)(0x00)(0x00)(0x00)
    uint8_t current = 0, nextpos = 0;

    while (current < sizeof(report_.keycodes)) {
      if (report_.keycodes[current]) {
        uint8_t tmp = report_.keycodes[nextpos];
        report_.keycodes[nextpos] = report_.keycodes[current];
        report_.keycodes[current] = tmp;
        ++nextpos;
      }
      ++current;
    }
  }

  return 1;
}


void BootKeyboard_::releaseAll() {
  memset(&report_.bytes, 0x00, sizeof(report_.bytes));
}


/* Returns true if the non-modifer key passed in will be sent during this key report
 * Returns false in all other cases
 * */
bool BootKeyboard_::isKeyPressed(uint8_t k) {
  for (uint8_t i = 0; i < sizeof(report_.keycodes); i++) {
    if (report_.keycodes[i] == k) {
      return true;
    }
  }
  return false;
}

/* Returns true if the non-modifer key passed in was sent during the previous key report
 * Returns false in all other cases
 * */
bool BootKeyboard_::wasKeyPressed(uint8_t k) {
  for (uint8_t i = 0; i < sizeof(report_.keycodes); i++) {
    if (last_report_.keycodes[i] == k) {
      return true;
    }
  }
  return false;
}



/* Returns true if the modifer key passed in will be sent during this key report
 * Returns false in all other cases
 * */
bool BootKeyboard_::isModifierActive(uint8_t k) {
  if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    return !!(report_.modifiers & (1 << k));
  }
  return false;
}

/* Returns true if the modifer key passed in was being sent during the previous key report
 * Returns false in all other cases
 * */
bool BootKeyboard_::wasModifierActive(uint8_t k) {
  if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    return !!(last_report_.modifiers & (1 << k));
  }
  return false;
}

/* Returns true if any modifier key will be sent during this key report
 * Returns false in all other cases
 * */
bool BootKeyboard_::isAnyModifierActive() {
  return report_.modifiers > 0;
}

/* Returns true if any modifier key was being sent during the previous key report
 * Returns false in all other cases
 * */
bool BootKeyboard_::wasAnyModifierActive() {
  return last_report_.modifiers > 0;
}

/*
 * Check whether the device has seen a bus reset. Unfortunately, the most
 * portable way is to poll for changes in the host-selected configuration. This
 * needs to be periodically called from the keyboard driver to poll for a reset
 * condition.
 */
void BootKeyboard_::checkReset() {
  static bool was_configed;

  if (was_configed) {
    if (!USB_Configured()) {
      was_configed = false;
      protocol = default_protocol;
    }
  } else {
    if (USB_Configured()) {
      was_configed = true;
    }
  }
}

__attribute__((weak))
BootKeyboard_& BootKeyboard() {
  static BootKeyboard_ obj;
  return obj;
};

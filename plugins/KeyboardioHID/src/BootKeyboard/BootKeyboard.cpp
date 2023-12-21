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
  D_USAGE_PAGE,
  D_PAGE_GENERIC_DESKTOP,
  D_USAGE,
  D_USAGE_KEYBOARD,

  D_COLLECTION,
  D_APPLICATION,

  // LEDs
  D_REPORT_COUNT,
  0x8,
  D_REPORT_SIZE,
  0x1,
  D_USAGE_PAGE,
  D_PAGE_LEDS,
  D_USAGE_MINIMUM,
  0x1,
  D_USAGE_MAXIMUM,
  0x8,
  D_LOGICAL_MINIMUM,
  0x0,
  D_LOGICAL_MAXIMUM,
  0x1,
  D_OUTPUT,
  (D_DATA | D_VARIABLE | D_ABSOLUTE),

  // Modifiers
  D_USAGE_PAGE,
  D_PAGE_KEYBOARD,
  D_USAGE_MINIMUM,
  HID_KEYBOARD_FIRST_MODIFIER,
  D_USAGE_MAXIMUM,
  HID_KEYBOARD_LAST_MODIFIER,
  // D_LOGICAL_MINIMUM, 0x0, // redundant; already 0
  // D_LOGICAL_MAXIMUM, 0x1, // redundant; already 1
  // D_REPORT_SIZE, 0x1, // redundant; already 1
  // D_REPORT_COUNT, 0x8, // redundant; already 8
  D_INPUT,
  (D_DATA | D_VARIABLE | D_ABSOLUTE),

  // Reserved byte
  D_REPORT_COUNT,
  0x1,
  D_REPORT_SIZE,
  0x8,
  D_INPUT,
  (D_CONSTANT),

  // Non-modifiers
  D_REPORT_COUNT,
  BOOT_KEY_BYTES,
  // D_REPORT_SIZE, 0x8, // redundant; already 8
  // D_LOGICAL_MINIMUM, HID_FIRST_KEY, // redundant; already 0
  D_MULTIBYTE(D_LOGICAL_MAXIMUM),
  HID_LAST_KEY,
  0x0,  // make sure it's not negative
  // D_USAGE_PAGE, D_PAGE_KEYBOARD, // redundant; already KEYBOARD
  D_USAGE_MINIMUM,
  HID_FIRST_KEY,
  D_USAGE_MAXIMUM,
  HID_LAST_KEY,
  D_INPUT,
  (D_DATA | D_ARRAY | D_ABSOLUTE),
  D_END_COLLECTION,
};

static const uint8_t hybrid_keyboard_hid_descriptor_[] PROGMEM = {
  //  Hybrid Boot/NKRO Keyboard
  D_USAGE_PAGE,
  D_PAGE_GENERIC_DESKTOP,
  D_USAGE,
  D_USAGE_KEYBOARD,

  D_COLLECTION,
  D_APPLICATION,

  /* 5 LEDs for num lock etc, 3 left for advanced, custom usage */
  D_USAGE_PAGE,
  D_PAGE_LEDS,
  D_USAGE_MINIMUM,
  0x01,
  D_USAGE_MAXIMUM,
  0x08,
  D_LOGICAL_MINIMUM,
  0x00,
  D_LOGICAL_MAXIMUM,
  0x01,
  D_REPORT_SIZE,
  0x01,
  D_REPORT_COUNT,
  0x08,
  D_OUTPUT,
  (D_DATA | D_VARIABLE | D_ABSOLUTE),

  D_USAGE_PAGE,
  D_PAGE_KEYBOARD,

  /* Key modifier byte for both boot and NKRO */
  D_USAGE_MINIMUM,
  HID_KEYBOARD_FIRST_MODIFIER,
  D_USAGE_MAXIMUM,
  HID_KEYBOARD_LAST_MODIFIER,
  // D_LOGICAL_MINIMUM, 0x00, // redundant; already 0
  // D_LOGICAL_MAXIMUM, 0x01, // redundant; already 1
  // D_REPORT_SIZE, 0x01, // redundant; already 1
  // D_REPORT_COUNT, 0x08, // redundant; already 8
  D_INPUT,
  (D_DATA | D_VARIABLE | D_ABSOLUTE),

  /* Send rest of boot report as padding, so HID-aware hosts will ignore */
  D_REPORT_SIZE,
  0x8,
  D_REPORT_COUNT,
  0x7,
  D_INPUT,
  (D_CONSTANT),

  /* NKRO key bitmap */

  // Padding 4 bits, to skip NO_EVENT & 3 error states.
  D_REPORT_SIZE,
  0x1,
  D_REPORT_COUNT,
  0x04,
  D_INPUT,
  (D_CONSTANT),

  // Actual non-modifier keys
  D_USAGE_MINIMUM,
  HID_KEYBOARD_A_AND_A,
  D_USAGE_MAXIMUM,
  HID_LAST_KEY,
  // D_LOGICAL_MINIMUM, 0x00, // redundant; already 0
  // D_LOGICAL_MAXIMUM, 0x01, // redundant; already 1
  // D_REPORT_SIZE, 0x01, // redundant; already 1
  D_REPORT_COUNT,
  (NKRO_KEY_BITS - 4),
  D_INPUT,
  (D_DATA | D_VARIABLE | D_ABSOLUTE),

#if (NKRO_KEY_BITS % 8)
  // Padding to round up the report to byte boundary.
  // D_REPORT_SIZE, 0x01, // redundant; already 1
  D_REPORT_COUNT,
  (8 - (NKRO_KEY_BITS % 8)),
  D_INPUT,
  (D_CONSTANT),
#endif

  D_END_COLLECTION,
};

BootKeyboard_::BootKeyboard_(uint8_t bootkb_only_)
  : PluggableUSBModule(1, 1, epType), protocol(HID_REPORT_PROTOCOL), idle(0), leds(0), bootkb_only(bootkb_only) {
#ifdef ARCH_HAS_CONFIGURABLE_EP_SIZES
  epType[0] = EP_TYPE_INTERRUPT_IN(USB_EP_SIZE);
#else
  epType[0] = EP_TYPE_INTERRUPT_IN;
#endif
  PluggableUSB().plug(this);
}

int BootKeyboard_::getInterface(uint8_t *interfaceCount) {
  *interfaceCount += 1;  // uses 1
  size_t desclen;
  if (bootkb_only) {
    desclen = sizeof(boot_keyboard_hid_descriptor_);
  } else {
    desclen = sizeof(hybrid_keyboard_hid_descriptor_);
  }
  HIDDescriptor hidInterface = {
    D_INTERFACE(pluggedInterface, 1, USB_DEVICE_CLASS_HUMAN_INTERFACE, HID_SUBCLASS_BOOT_INTERFACE, HID_PROTOCOL_KEYBOARD),
    D_HIDREPORT(desclen),
    D_ENDPOINT(USB_ENDPOINT_IN(pluggedEndpoint), USB_ENDPOINT_TYPE_INTERRUPT, USB_EP_SIZE, 0x01),
  };
  return USB_SendControl(0, &hidInterface, sizeof(hidInterface));
}

int BootKeyboard_::getDescriptor(USBSetup &setup) {
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

  if (bootkb_only) {
    return USB_SendControl(TRANSFER_PGM, boot_keyboard_hid_descriptor_, sizeof(boot_keyboard_hid_descriptor_));
  } else {
    return USB_SendControl(TRANSFER_PGM, hybrid_keyboard_hid_descriptor_, sizeof(hybrid_keyboard_hid_descriptor_));
  }
}


void BootKeyboard_::begin() {
}


void BootKeyboard_::end() {
  releaseAll();
  sendReport();
}


bool BootKeyboard_::setup(USBSetup &setup) {
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
      // TODO(anyone) improve
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
      // TODO(anyone) improve
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

      if (setup.wValueH == HID_REPORT_TYPE_OUTPUT) {
        if (length == sizeof(leds)) {
          USB_RecvControl(&leds, length);
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

uint8_t BootKeyboard_::getBootOnly() {
  return bootkb_only;
}

/*
 * Set whether to send only Boot Protocol regardless of whether Boot
 * Protocol has been requested. Used by USBQuirks to toggle protocol
 * modes.
 *
 * This is only really safe to call after detaching from USB, because
 * otherwise, the report format might get out of sync with what the host
 * expects, and the host probably won't see the new Report Descriptor
 * until after a re-attach.
 */
void BootKeyboard_::setBootOnly(uint8_t bootonly) {
  bootkb_only = bootonly;
}


void BootKeyboard_::convertReport(uint8_t *boot, const uint8_t *nkro) {
  uint8_t n_boot_keys = 0;
  uint8_t b;
  // Convert NKRO report to boot report
  memset(boot, HID_KEYBOARD_NO_EVENT, BOOT_KEY_BYTES);
  for (uint8_t i = 0; i < NKRO_KEY_BYTES; i++) {
    b = nkro[i];
    if (b == 0) {
      continue;
    }
    for (uint8_t j = 0; j < 8; j++) {
      bool bit = b & 1;
      b >>= 1;
      if (bit == 0) {
        continue;
      }
      // Check is here so we can set all BOOT_KEY_BYTES
      if (n_boot_keys >= BOOT_KEY_BYTES) {
        // Send rollover error if too many keys are held
        memset(boot, HID_KEYBOARD_ERROR_ROLLOVER, BOOT_KEY_BYTES);
        return;
      }
      boot[n_boot_keys++] = 8 * i + j;
    }
  }
}

/* Send a report without the extra modifier change handling */
int BootKeyboard_::sendReportUnchecked() {
  HID_BootKeyboardReport_Data_t out_report;
  out_report.modifiers = last_report_.modifiers;
  out_report.reserved  = 0;
  memcpy(out_report.nkro_keys, last_report_.keys, sizeof(last_report_.keys));
  convertReport(out_report.boot_keycodes, out_report.nkro_keys);
  size_t reportlen;
  // Send only boot report if host requested boot protocol, or if configured as boot-only
  if (protocol == HID_BOOT_PROTOCOL || bootkb_only) {
    reportlen = BOOT_REPORT_LEN;
  } else {
    reportlen = sizeof(out_report);
  }
  int returnCode = USB_Send(pluggedEndpoint | TRANSFER_RELEASE, &out_report, reportlen);
  HIDReportObserver::observeReport(HID_REPORTID_KEYBOARD, &out_report, reportlen, returnCode);
  return returnCode;
}

// Sending the current HID report to the host:
//
// Depending on the differences between the current and previous HID reports, we
// might need to send one or two extra reports to guarantee that the host will
// process the changes in the correct order. There are two important scenarios
// to consider:
//
// 1. If a non-modifier keycode toggles off in the same report as a modifier
// changes, the host might process the modifier change first. For example, if
// both `shift` and `4` toggle off in the same report (most likely from a
// `LSHIFT(Key_4)` key being released), and that key has been held long enough
// to trigger character repeat, we could end up with a plain `4` in the output
// at the end of the repeat: `$$$$4` instead of `$$$$$`.
//
// 2. If a non-modifier keycode toggles on in the same report as a modifier
// changes, the host might process the non-modifer first. For example, pressing
// and holding an `LSHIFT(Key_4)` key might result in `4$$$` rather than `$$$$`.
//
// Therefore, each call to `sendReport()` must send (up to) three reports to the
// host to guarantee the correct order of processing:
//
// 1. A report with toggled-off non-modifiers removed.
// 2. A report with changes to modifiers.
// 3. A report with toggled-on non-modifiers added.

int BootKeyboard_::sendReport() {
  // If the new HID report differs from the previous one both in active modifier
  // keycodes and non-modifier keycodes, we will need to send at least one extra
  // report. First, we compare the modifiers bytes of the two reports.
  const uint8_t old_modifiers = last_report_.modifiers;
  const uint8_t new_modifiers = report_.modifiers;

  const uint8_t changed_modifiers = old_modifiers ^ new_modifiers;

  if (changed_modifiers != 0) {
    // There was at least one modifier change (toggled on or off), remove any
    // non-modifiers from the stored previous report that toggled off in the new
    // report, and send it to the host.
    bool non_modifiers_toggled_off = false;
    for (uint8_t i = 0; i < NKRO_KEY_BYTES; ++i) {
      byte released_keycodes = last_report_.keys[i] & ~(report_.keys[i]);
      if (released_keycodes != 0) {
        last_report_.keys[i] &= ~released_keycodes;
        non_modifiers_toggled_off = true;
      }
    }
    if (non_modifiers_toggled_off) {
      sendReportUnchecked();
    }
    // Next, update the modifiers byte of the stored previous report, and send
    // it.
    last_report_.modifiers = new_modifiers;
    sendReportUnchecked();
  }

  // Finally, copy the new report to the previous one, and send it.
  if (memcmp(last_report_.keys, report_.keys, sizeof(report_.keys)) != 0) {
    memcpy(last_report_.keys, report_.keys, sizeof(report_.keys));
    return sendReportUnchecked();
  }
  // A note on return values: Kaleidoscope doesn't actually check the return
  // value of `sendReport()`, so this function could be changed to return
  // void. It would be nice if we could do something to recover from an error
  // here, but it's not at all clear what that should be. Also note that if the
  // extra reports above return an error, there's not much we can do to try to
  // recover. We could try to send the report again, but that would be likely to
  // fail as well.
  return -1;
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

/* Returns true if *any* modifier will be sent during this key report
 * Returns false in all other cases
 * */
bool BootKeyboard_::isAnyModifierActive() {
  return report_.modifiers > 0;
}

/* Returns true if *any* modifier was being sent during the previous key report
 * Returns false in all other cases
 * */
bool BootKeyboard_::wasAnyModifierActive() {
  return last_report_.modifiers > 0;
}


/* Returns true if the non-modifier key passed in will be sent during this key report
 * Returns false in all other cases
 * */
bool BootKeyboard_::isKeyPressed(uint8_t k) {
  if (k <= HID_LAST_KEY) {
    uint8_t bit = 1 << (uint8_t(k) % 8);
    return !!(report_.keys[k / 8] & bit);
  }
  return false;
}

/* Returns true if the non-modifer key passed in was sent during the previous key report
 * Returns false in all other cases
 * */
bool BootKeyboard_::wasKeyPressed(uint8_t k) {

  if (k <= HID_LAST_KEY) {
    uint8_t bit = 1 << (uint8_t(k) % 8);
    return !!(last_report_.keys[k / 8] & bit);
  }
  return false;
}


size_t BootKeyboard_::press(uint8_t k) {
  // If the key is in the range of 'printable' keys
  if (k <= HID_LAST_KEY) {
    uint8_t bit = 1 << (uint8_t(k) % 8);
    report_.keys[k / 8] |= bit;
    return 1;
  } else if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    // It's a modifier key
    // Convert key into bitfield (0 - 7)
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    report_.modifiers |= (1 << k);
    return 1;
  }

  // No empty/pressed key was found
  return 0;
}

size_t BootKeyboard_::release(uint8_t k) {
  // If we're releasing a printable key
  if (k <= HID_LAST_KEY) {
    uint8_t bit = 1 << (k % 8);
    report_.keys[k / 8] &= ~bit;
    return 1;
  } else if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    // It's a modifier key
    // Convert key into bitfield (0 - 7)
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    report_.modifiers &= ~(1 << k);
    return 1;
  }

  // No empty/pressed key was found
  return 0;
}

void BootKeyboard_::releaseAll() {
  // Release all keys
  memset(&report_.allkeys, 0x00, sizeof(report_.allkeys));
}

/*
 * Hook function to reset any needed state after a USB reset.
 *
 * Right now, it sets the protocol back to the default (report protocol), as
 * required by the HID specification.
 */
void BootKeyboard_::onUSBReset() {
  protocol = HID_REPORT_PROTOCOL;
}

__attribute__((weak))
BootKeyboard_ &
BootKeyboard() {
  static BootKeyboard_ obj;
  return obj;
}

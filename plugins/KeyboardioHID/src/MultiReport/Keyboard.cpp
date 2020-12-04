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

#include "Keyboard.h"
#include "DescriptorPrimitives.h"

static const uint8_t nkro_keyboard_hid_descriptor_[] PROGMEM = {
  //  NKRO Keyboard
  D_USAGE_PAGE, D_PAGE_GENERIC_DESKTOP,
  D_USAGE, D_USAGE_KEYBOARD,
  D_COLLECTION, D_APPLICATION,
  D_REPORT_ID, HID_REPORTID_NKRO_KEYBOARD,
  D_USAGE_PAGE, D_PAGE_KEYBOARD,


  /* Key modifier byte */
  D_USAGE_MINIMUM, HID_KEYBOARD_FIRST_MODIFIER,
  D_USAGE_MAXIMUM, HID_KEYBOARD_LAST_MODIFIER,
  D_LOGICAL_MINIMUM, 0x00,
  D_LOGICAL_MAXIMUM, 0x01,
  D_REPORT_SIZE, 0x01,
  D_REPORT_COUNT, 0x08,
  D_INPUT, (D_DATA | D_VARIABLE | D_ABSOLUTE),


  /* 5 LEDs for num lock etc, 3 left for advanced, custom usage */
  D_USAGE_PAGE, D_PAGE_LEDS,
  D_USAGE_MINIMUM, 0x01,
  D_USAGE_MAXIMUM, 0x08,
  D_REPORT_COUNT, 0x08,
  D_REPORT_SIZE, 0x01,
  D_OUTPUT, (D_DATA | D_VARIABLE | D_ABSOLUTE),

  /* NKRO Keyboard */
  D_USAGE_PAGE, D_PAGE_KEYBOARD,

  // Padding 4 bits, to skip NO_EVENT & 3 error states.
  D_REPORT_SIZE, 0x04,
  D_REPORT_COUNT, 0x01,
  D_INPUT, (D_CONSTANT),

  D_USAGE_MINIMUM, HID_KEYBOARD_A_AND_A,
  D_USAGE_MAXIMUM, HID_LAST_KEY,
  D_LOGICAL_MINIMUM, 0x00,
  D_LOGICAL_MAXIMUM, 0x01,
  D_REPORT_SIZE, 0x01,
  D_REPORT_COUNT, (HID_LAST_KEY - HID_KEYBOARD_A_AND_A),
  D_INPUT, (D_DATA | D_VARIABLE | D_ABSOLUTE),

  // Padding (3 bits) to round up the report to byte boundary.
  D_REPORT_SIZE, 0x03,
  D_REPORT_COUNT, 0x01,
  D_INPUT, (D_CONSTANT),

  D_END_COLLECTION,

};

Keyboard_::Keyboard_() {
  static HIDSubDescriptor node(nkro_keyboard_hid_descriptor_, sizeof(nkro_keyboard_hid_descriptor_));
  HID().AppendDescriptor(&node);
}

void Keyboard_::begin() {
  // Force API to send a clean report.
  // This is important for and HID bridge where the receiver stays on,
  // while the sender is resetted.
  releaseAll();
  sendReportUnchecked();
}


void Keyboard_::end() {
  releaseAll();
  sendReportUnchecked();
}

int Keyboard_::sendReportUnchecked() {
  return HID().SendReport(HID_REPORTID_NKRO_KEYBOARD, &report_, sizeof(report_));
}


int Keyboard_::sendReport() {
  // If the last report is different than the current report, then we need to send a report.
  // We guard sendReport like this so that calling code doesn't end up spamming the host with empty reports
  // if sendReport is called in a tight loop.

  if (memcmp(last_report_.allkeys, report_.allkeys, sizeof(report_))) {
    // if the two reports are different, send a report

    // ChromeOS 51-60 (at least) bug: if a modifier and a normal keycode are added in the
    // same report, in some cases the shift is not applied (e.g. `shift`+`[` doesn't yield
    // `{`). To compensate for this, check to see if the modifier byte has changed.


    // If modifiers are being turned on at the same time as any change
    // to the non-modifier keys in the report, then we send the previous
    // report with the new modifiers
    if (((last_report_.modifiers ^ report_.modifiers) & report_.modifiers)
        && (memcmp(last_report_.keys, report_.keys, sizeof(report_.keys)))) {
      uint8_t last_mods = last_report_.modifiers;
      last_report_.modifiers = report_.modifiers;
      int returnCode = HID().SendReport(HID_REPORTID_NKRO_KEYBOARD, &last_report_, sizeof(last_report_));
      last_report_.modifiers = last_mods;
    }

    // If modifiers are being turned off, then we send the new report with the previous modifiers.
    // We need to do this, at least on Linux 4.17 + Wayland.
    // Jesse has observed that sending Shift + 3 key up events in the same report
    // will sometimes result in a spurious '3' rather than '#', especially when the keys
    // had been held for a while
    else if (((last_report_.modifiers ^ report_.modifiers) & last_report_.modifiers)
             && (memcmp(last_report_.keys, report_.keys, sizeof(report_.keys)))) {
      uint8_t mods = report_.modifiers;
      report_.modifiers = last_report_.modifiers;
      int returnCode = HID().SendReport(HID_REPORTID_NKRO_KEYBOARD, &report_, sizeof(last_report_));
      report_.modifiers = mods;
    }





    int returnCode = sendReportUnchecked();
    if (returnCode > 0)
      memcpy(last_report_.allkeys, report_.allkeys, sizeof(report_));
    return returnCode;
  }
  return -1;
}

/* Returns true if the modifer key passed in will be sent during this key report
 * Returns false in all other cases
 * */
bool Keyboard_::isModifierActive(uint8_t k) {
  if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    return !!(report_.modifiers & (1 << k));
  }
  return false;
}

/* Returns true if the modifer key passed in was being sent during the previous key report
 * Returns false in all other cases
 * */
bool Keyboard_::wasModifierActive(uint8_t k) {
  if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    return !!(last_report_.modifiers & (1 << k));
  }
  return false;
}

/* Returns true if *any* modifier will be sent during this key report
 * Returns false in all other cases
 * */
bool Keyboard_::isAnyModifierActive() {
  return report_.modifiers > 0;
}

/* Returns true if *any* modifier was being sent during the previous key report
 * Returns false in all other cases
 * */
bool Keyboard_::wasAnyModifierActive() {
  return last_report_.modifiers > 0;
}


/* Returns true if the non-modifier key passed in will be sent during this key report
 * Returns false in all other cases
 * */
bool Keyboard_::isKeyPressed(uint8_t k) {
  if (k <= HID_LAST_KEY) {
    uint8_t bit = 1 << (uint8_t(k) % 8);
    return !!(report_.keys[k / 8] & bit);
  }
  return false;
}

/* Returns true if the non-modifer key passed in was sent during the previous key report
 * Returns false in all other cases
 * */
bool Keyboard_::wasKeyPressed(uint8_t k) {

  if (k <= HID_LAST_KEY) {
    uint8_t bit = 1 << (uint8_t(k) % 8);
    return !!(last_report_.keys[k / 8] & bit);
  }
  return false;
}


size_t Keyboard_::press(uint8_t k) {
  // If the key is in the range of 'printable' keys
  if (k <= HID_LAST_KEY) {
    uint8_t bit = 1 << (uint8_t(k) % 8);
    report_.keys[k / 8] |= bit;
    return 1;
  }

  // It's a modifier key
  else if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    // Convert key into bitfield (0 - 7)
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    report_.modifiers |= (1 << k);
    return 1;
  }

  // No empty/pressed key was found
  return 0;
}

size_t Keyboard_::release(uint8_t k) {
  // If we're releasing a printable key
  if (k <= HID_LAST_KEY) {
    uint8_t bit = 1 << (k % 8);
    report_.keys[k / 8] &= ~bit;
    return 1;
  }

  // It's a modifier key
  else if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    // Convert key into bitfield (0 - 7)
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    report_.modifiers &= ~(1 << k);
    return 1;
  }

  // No empty/pressed key was found
  return 0;
}

void Keyboard_::releaseAll() {
  // Release all keys
  memset(&report_.allkeys, 0x00, sizeof(report_.allkeys));
}

Keyboard_ Keyboard;

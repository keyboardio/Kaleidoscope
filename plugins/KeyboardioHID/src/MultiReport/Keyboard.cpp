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
  static HIDSubDescriptor node(nkro_keyboard_hid_descriptor_,
                               sizeof(nkro_keyboard_hid_descriptor_));
  HID().AppendDescriptor(&node);
}

void Keyboard_::begin() {
  // Force API to send a clean report.  This is important for and HID bridge
  // where the receiver stays on, while the sender is resetted.
  releaseAll();
  sendReportUnchecked();
}


void Keyboard_::end() {
  releaseAll();
  sendReportUnchecked();
}

int Keyboard_::sendReportUnchecked() {
  return HID().SendReport(HID_REPORTID_NKRO_KEYBOARD,
                          &last_report_, sizeof(last_report_));
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

int Keyboard_::sendReport() {
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
    for (uint8_t i = 0; i < KEY_BYTES; ++i) {
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

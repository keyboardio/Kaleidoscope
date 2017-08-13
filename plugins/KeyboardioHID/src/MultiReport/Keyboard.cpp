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

#include "Keyboard.h"
#include "DescriptorPrimitives.h"

static const uint8_t _hidMultiReportDescriptorKeyboard[] PROGMEM = {
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
  D_INPUT, (D_DATA|D_VARIABLE|D_ABSOLUTE),

// USB Code not within 4-49 (0x4-0x31), 51-155 (0x33-0x9B), 157-164 (0x9D-0xA4),
// 176-221 (0xB0-0xDD) or 224-231 (0xE0-0xE7) NKRO Mode
  /* NKRO Keyboard */

  // Padding 3 bits
  // To skip HID_KEYBOARD_NON_US_POUND_AND_TILDE, which causes
  // Linux to choke on our driver.
  D_REPORT_SIZE, 0x04,
  D_REPORT_COUNT, 0x01,
  D_INPUT, (D_CONSTANT),


  D_USAGE_MINIMUM, HID_KEYBOARD_A_AND_A,
  D_USAGE_MAXIMUM, HID_KEYBOARD_BACKSLASH_AND_PIPE,
  D_LOGICAL_MINIMUM, 0x00,
  D_LOGICAL_MAXIMUM, 0x01,
  D_REPORT_SIZE, 0x01,
  D_REPORT_COUNT, (HID_KEYBOARD_BACKSLASH_AND_PIPE - HID_KEYBOARD_A_AND_A)+1,
  D_INPUT, (D_DATA|D_VARIABLE|D_ABSOLUTE),

  // Padding 1 bit.
  // To skip HID_KEYBOARD_NON_US_POUND_AND_TILDE, which causes
  // Linux to choke on our driver.
  D_REPORT_SIZE, 0x01,
  D_REPORT_COUNT, 0x01,
  D_INPUT, (D_CONSTANT),


  D_USAGE_MINIMUM, HID_KEYBOARD_SEMICOLON_AND_COLON,
  D_USAGE_MAXIMUM, HID_KEYBOARD_CANCEL,
  D_LOGICAL_MINIMUM, 0x00,
  D_LOGICAL_MAXIMUM, 0x01,
  D_REPORT_SIZE, 0x01,
  D_REPORT_COUNT, (HID_KEYBOARD_CANCEL-HID_KEYBOARD_SEMICOLON_AND_COLON) +1,
  D_INPUT, (D_DATA|D_VARIABLE|D_ABSOLUTE),


  // Padding 1 bit.
  // To skip HID_KEYBOARD_CLEAR, which causes
  // Linux to choke on our driver.
  D_REPORT_SIZE, 0x01,
  D_REPORT_COUNT, 0x01,
  D_INPUT, (D_CONSTANT),

  D_USAGE_MINIMUM, HID_KEYBOARD_PRIOR,
  D_USAGE_MAXIMUM, HID_KEYPAD_HEXADECIMAL,
  D_LOGICAL_MINIMUM, 0x00,
  D_LOGICAL_MAXIMUM, 0x01,
  D_REPORT_SIZE, 0x01,
  D_REPORT_COUNT, (HID_KEYPAD_HEXADECIMAL - HID_KEYBOARD_PRIOR)  +1,
  D_INPUT, (D_DATA|D_VARIABLE|D_ABSOLUTE),


  // Padding (w bits)
  D_REPORT_SIZE, 0x02,
  D_REPORT_COUNT, 0x01,
  D_INPUT, (D_CONSTANT),

  D_END_COLLECTION,

};

Keyboard_::Keyboard_(void) {
  static HIDSubDescriptor node(_hidMultiReportDescriptorKeyboard, sizeof(_hidMultiReportDescriptorKeyboard));
  HID().AppendDescriptor(&node);
}

void Keyboard_::begin(void) {
  // Force API to send a clean report.
  // This is important for and HID bridge where the receiver stays on,
  // while the sender is resetted.
  releaseAll();
  sendReportUnchecked();
}


void Keyboard_::end(void) {
  releaseAll();
  sendReportUnchecked();
}

int Keyboard_::sendReportUnchecked(void) {
    return HID().SendReport(HID_REPORTID_NKRO_KEYBOARD, &_keyReport, sizeof(_keyReport));
}


int Keyboard_::sendReport(void) {
  // If the last report is different than the current report, then we need to send a report.
  // We guard sendReport like this so that calling code doesn't end up spamming the host with empty reports
  // if sendReport is called in a tight loop.

  if (memcmp(_lastKeyReport.allkeys, _keyReport.allkeys, sizeof(_keyReport))) {
    // if the two reports are different, send a report
    int returnCode = sendReportUnchecked();
    memcpy(_lastKeyReport.allkeys, _keyReport.allkeys, sizeof(_keyReport));
    return returnCode;
  }
  return -1;
}

/* Returns true if the modifer key passed in will be sent during this key report
 * Returns false in all other cases
 * */
boolean Keyboard_::isModifierActive(uint8_t k) {
  if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    return !!(_keyReport.modifiers & (1 << k));
  }
  return false;
}

/* Returns true if the modifer key passed in was being sent during the previous key report
 * Returns false in all other cases
 * */
boolean Keyboard_::wasModifierActive(uint8_t k) {
  if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    return !!(_lastKeyReport.modifiers & (1 << k));
  }
  return false;
}

size_t Keyboard_::press(uint8_t k) {
  // If the key is in the range of 'printable' keys
  if (k <= HID_LAST_KEY) {
    uint8_t bit = 1 << (uint8_t(k) % 8);
    _keyReport.keys[k / 8] |= bit;
    return 1;
  }

  // It's a modifier key
  else if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    // Convert key into bitfield (0 - 7)
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    _keyReport.modifiers |= (1 << k);
    return 1;
  }

  // No empty/pressed key was found
  return 0;
}

size_t Keyboard_::release(uint8_t k) {
  // If we're releasing a printable key
  if (k <= HID_LAST_KEY) {
    uint8_t bit = 1 << (k % 8);
    _keyReport.keys[k / 8] &= ~bit;
    return 1;
  }

  // It's a modifier key
  else if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    // Convert key into bitfield (0 - 7)
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    _keyReport.modifiers &= ~(1 << k);
    return 1;
  }

  // No empty/pressed key was found
  return 0;
}

void Keyboard_::releaseAll(void) {
  // Release all keys
  memset(&_keyReport.allkeys, 0x00, sizeof(_keyReport.allkeys));
}

Keyboard_ Keyboard;

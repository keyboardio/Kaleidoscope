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
    _USAGE_PAGE, _PAGE_GENERIC_DESKTOP,
    _USAGE, _USAGE_KEYBOARD,
    _COLLECTION, _APPLICATION,
    _REPORT_ID, HID_REPORTID_NKRO_KEYBOARD,
    _USAGE_PAGE, _PAGE_KEYBOARD,


    /* Key modifier byte */
    _USAGE_MINIMUM, HID_KEYBOARD_FIRST_MODIFIER,
    _USAGE_MAXIMUM, HID_KEYBOARD_LAST_MODIFIER,
    _LOGICAL_MINIMUM, 0x00,
    _LOGICAL_MAXIMUM, 0x01,
    _REPORT_SIZE, 0x01,
    _REPORT_COUNT, 0x08,
    _INPUT, (_DATA|_VARIABLE|_ABSOLUTE),

// USB Code not within 4-49 (0x4-0x31), 51-155 (0x33-0x9B), 157-164 (0x9D-0xA4),
// 176-221 (0xB0-0xDD) or 224-231 (0xE0-0xE7) NKRO Mode
    /* NKRO Keyboard */

    // Padding 3 bits
    // To skip HID_KEYBOARD_NON_US_POUND_AND_TILDE, which causes
    // Linux to choke on our driver.
    _REPORT_SIZE, 0x04,
    _REPORT_COUNT, 0x01,
    _INPUT, (_CONSTANT),


    _USAGE_MINIMUM, HID_KEYBOARD_A_AND_A,
    _USAGE_MAXIMUM, HID_KEYBOARD_BACKSLASH_AND_PIPE,
    _LOGICAL_MINIMUM, 0x00,
    _LOGICAL_MAXIMUM, 0x01,
    _REPORT_SIZE, 0x01,
    _REPORT_COUNT, (HID_KEYBOARD_BACKSLASH_AND_PIPE - HID_KEYBOARD_A_AND_A)+1,
    _INPUT, (_DATA|_VARIABLE|_ABSOLUTE),

    // Padding 1 bit.
    // To skip HID_KEYBOARD_NON_US_POUND_AND_TILDE, which causes
    // Linux to choke on our driver.
    _REPORT_SIZE, 0x01,
    _REPORT_COUNT, 0x01,
    _INPUT, (_CONSTANT),


    _USAGE_MINIMUM, HID_KEYBOARD_SEMICOLON_AND_COLON,
    _USAGE_MAXIMUM, HID_KEYBOARD_CANCEL,
    _LOGICAL_MINIMUM, 0x00,
    _LOGICAL_MAXIMUM, 0x01,
    _REPORT_SIZE, 0x01,
    _REPORT_COUNT, (HID_KEYBOARD_CANCEL-HID_KEYBOARD_SEMICOLON_AND_COLON) +1,
    _INPUT, (_DATA|_VARIABLE|_ABSOLUTE),


    // Padding 1 bit.
    // To skip HID_KEYBOARD_CLEAR, which causes
    // Linux to choke on our driver.
    _REPORT_SIZE, 0x01,
    _REPORT_COUNT, 0x01,
    _INPUT, (_CONSTANT),

    _USAGE_MINIMUM, HID_KEYBOARD_PRIOR,
    _USAGE_MAXIMUM, HID_KEYPAD_HEXADECIMAL,
    _LOGICAL_MINIMUM, 0x00,
    _LOGICAL_MAXIMUM, 0x01,
    _REPORT_SIZE, 0x01,
    _REPORT_COUNT, (HID_KEYPAD_HEXADECIMAL - HID_KEYBOARD_PRIOR )  +1 ,
    _INPUT, (_DATA|_VARIABLE|_ABSOLUTE),


    // Padding (w bits)
    _REPORT_SIZE, 0x02,
    _REPORT_COUNT, 0x01,
    _INPUT, (_CONSTANT),

    _END_COLLECTION,

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
    sendReport();
}


void Keyboard_::end(void) {
    releaseAll();
    sendReport();
}



int Keyboard_::sendReport(void) {
    return HID().SendReport(HID_REPORTID_NKRO_KEYBOARD, &_keyReport, sizeof(_keyReport));
}

size_t Keyboard_::press(uint8_t k) {
    // If the key is in the range of 'printable' keys 
    if (k <= HID_KEYPAD_HEXADECIMAL ) {
        uint8_t bit = 1 << (uint8_t(k) % 8);
        _keyReport.keys[k / 8] |= bit;
        return 1;
    }

    // It's a modifier key
    else if(k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
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
    if (k <= HID_KEYPAD_HEXADECIMAL) {
        uint8_t bit = 1 << (k % 8);
        _keyReport.keys[k / 8] &= ~bit;
        return 1;
    }

    // It's a modifier key
    else if(k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
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



size_t Keyboard_::write(uint8_t k) {
    if(k >= sizeof(_asciimap)) // Ignore invalid input
        return 0;

    // Read key from ascii lookup table
    k = pgm_read_byte(_asciimap + k);

    if(k & SHIFT)
        press(HID_KEYBOARD_LEFT_SHIFT);
    press(k & ~SHIFT);
    sendReport();
    releaseAll();
    sendReport();
    return 1;
}



Keyboard_ Keyboard;


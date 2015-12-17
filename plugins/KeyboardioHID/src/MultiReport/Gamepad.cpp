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

#include "Gamepad.h"
#include "DescriptorPrimitives.h"

static const uint8_t _hidMultiReportDescriptorGamepad[] PROGMEM = {
    /* Gamepad with 32 buttons and 6 axis*/
    _USAGE_PAGE, _PAGE_GENERIC_DESKTOP,							/* USAGE_PAGE (Generic Desktop) */
    _USAGE, _USAGE_JOYSTICK,							/* USAGE (Joystick) */
    _COLLECTION, _APPLICATION,							/* COLLECTION (Application) */
    _REPORT_ID, HID_REPORTID_GAMEPAD,			/*   REPORT_ID */
    /* 32 Buttons */
    _USAGE_PAGE, _PAGE_BUTTON,							/*   USAGE_PAGE (Button) */
    _USAGE_MINIMUM, 0x01,							/*   USAGE_MINIMUM (Button 1) */
    _USAGE_MAXIMUM, 0x20,							/*   USAGE_MAXIMUM (Button 32) */
    _LOGICAL_MINIMUM, 0x00,							/*   _LOGICAL_MINIMUM (0) */
    _LOGICAL_MAXIMUM, 0x01,							/*   _LOGICAL_MAXIMUM (1) */
    _REPORT_SIZE, 0x01,							/*   REPORT_SIZE (1) */
    _REPORT_COUNT, 0x20,							/*   REPORT_COUNT (32) */
    _INPUT, (_DATA|_VARIABLE|_ABSOLUTE),							/*   INPUT (Data,Var,Abs) */
    /* 4 16bit Axis */
    _USAGE_PAGE, _PAGE_GENERIC_DESKTOP,							/*   USAGE_PAGE (Generic Desktop) */
    _COLLECTION, _PHYSICAL,							/*   COLLECTION (Physical) */
    _USAGE, 0x30,							/*     USAGE (X) */
    _USAGE, 0x31,							/*     USAGE (Y) */
    _USAGE, 0x33,							/*     USAGE (Rx) */
    _USAGE, 0x34,							/*     USAGE (Ry) */
    _MULTIBYTE(_LOGICAL_MINIMUM), 0x00, 0x80,					/*     _LOGICAL_MINIMUM (-32768) */
    _MULTIBYTE(_LOGICAL_MAXIMUM), 0xFF, 0x7F,					/*     _LOGICAL_MAXIMUM (32767) */
    _REPORT_SIZE, 0x10,							/*     REPORT_SIZE (16) */
    _REPORT_COUNT, 0x04,							/*     REPORT_COUNT (4) */
    _INPUT, (_DATA|_VARIABLE|_ABSOLUTE),							/*     INPUT (Data,Var,Abs) */
    /* 2 8bit Axis */
    _USAGE, 0x32,							/*     USAGE (Z) */
    _USAGE, 0x35,							/*     USAGE (Rz) */
    _LOGICAL_MINIMUM, 0x80,							/*     _LOGICAL_MINIMUM (-128) */
    _LOGICAL_MAXIMUM, 0x7F,							/*     _LOGICAL_MAXIMUM (127) */
    _REPORT_SIZE, 0x08,							/*     REPORT_SIZE (8) */
    _REPORT_COUNT, 0x02,							/*     REPORT_COUNT (2) */
    _INPUT, (_DATA|_VARIABLE|_ABSOLUTE),							/*     INPUT (Data,Var,Abs) */
    _END_COLLECTION,								/*   END_COLLECTION */

    /* 2 Hat Switches */
    _USAGE_PAGE, _PAGE_GENERIC_DESKTOP,							/*   USAGE_PAGE (Generic Desktop) */
    _USAGE, 0x39,							/*   USAGE (Hat switch) */
    _USAGE, 0x39,							/*   USAGE (Hat switch) */
    _LOGICAL_MINIMUM, 0x01,							/*   _LOGICAL_MINIMUM (1) */
    _LOGICAL_MAXIMUM, 0x08,							/*   _LOGICAL_MAXIMUM (8) */
    _REPORT_COUNT, 0x02,							/*   REPORT_COUNT (2) */
    _REPORT_SIZE, 0x04,							/*   REPORT_SIZE (4) */
    _INPUT, (_DATA|_VARIABLE|_ABSOLUTE),							/*   INPUT (Data,Var,Abs) */
    _END_COLLECTION								/* END_COLLECTION */
};

Gamepad_::Gamepad_(void) {
    static HIDSubDescriptor node(_hidMultiReportDescriptorGamepad, sizeof(_hidMultiReportDescriptorGamepad));
    HID().AppendDescriptor(&node);
}


void Gamepad_::begin(void) {
    // release all buttons
    end();
}

void Gamepad_::end(void) {
    memset(&_report, 0x00, sizeof(_report));
    SendReport(&_report, sizeof(_report));
}

void Gamepad_::write(void) {
    SendReport(&_report, sizeof(_report));
}


void Gamepad_::press(uint8_t b) {
    _report.buttons |= (uint32_t)1 << (b - 1);
}


void Gamepad_::release(uint8_t b) {
    _report.buttons &= ~((uint32_t)1 << (b - 1));
}


void Gamepad_::releaseAll(void) {
    memset(&_report, 0x00, sizeof(_report));
}

void Gamepad_::buttons(uint32_t b) {
    _report.buttons = b;
}


void Gamepad_::xAxis(int16_t a) {
    _report.xAxis = a;
}


void Gamepad_::yAxis(int16_t a) {
    _report.yAxis = a;
}


void Gamepad_::zAxis(int8_t a) {
    _report.zAxis = a;
}


void Gamepad_::rxAxis(int16_t a) {
    _report.rxAxis = a;
}


void Gamepad_::ryAxis(int16_t a) {
    _report.ryAxis = a;
}


void Gamepad_::rzAxis(int8_t a) {
    _report.rzAxis = a;
}


void Gamepad_::dPad1(int8_t d) {
    _report.dPad1 = d;
}


void Gamepad_::dPad2(int8_t d) {
    _report.dPad2 = d;
}


void Gamepad_::SendReport(void* data, int length) {
    HID().SendReport(HID_REPORTID_GAMEPAD, data, length);
}

Gamepad_ Gamepad;


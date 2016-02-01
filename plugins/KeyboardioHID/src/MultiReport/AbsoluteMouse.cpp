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

#include "AbsoluteMouse.h"
#include "DescriptorPrimitives.h"

static const uint8_t _hidMultiReportDescriptorAbsoluteMouse[] PROGMEM = {
    /*  Mouse absolute */
    _USAGE_PAGE, _PAGE_GENERIC_DESKTOP,                      /* USAGE_PAGE (Generic Desktop)	  54 */
    _USAGE, _USAGE_MOUSE,                      /* USAGE (Mouse) */
    _COLLECTION, _APPLICATION,                      /* COLLECTION (Application) */
    _REPORT_ID, HID_REPORTID_MOUSE_ABSOLUTE,	/*     REPORT_ID */

    /* 8 Buttons */
    _USAGE_PAGE, _PAGE_BUTTON,                      /*     USAGE_PAGE (Button) */
    _USAGE_MINIMUM, 0x01,                      /*     USAGE_MINIMUM (Button 1) */
    _USAGE_MAXIMUM, 0x08,                      /*     USAGE_MAXIMUM (Button 8) */
    _LOGICAL_MINIMUM, 0x00,                      /*     LOGICAL_MINIMUM (0) */
    _LOGICAL_MAXIMUM, 0x01,                      /*     LOGICAL_MAXIMUM (1) */
    _REPORT_COUNT, 0x08,                      /*     REPORT_COUNT (8) */
    _REPORT_SIZE, 0x01,                      /*     REPORT_SIZE (1) */
    _INPUT, (_DATA|_VARIABLE|_ABSOLUTE),

    /* X, Y */
    _USAGE_PAGE, _PAGE_GENERIC_DESKTOP,                      /*     USAGE_PAGE (Generic Desktop) */
    _USAGE, 0x30,                      /*     USAGE (X) */
    _USAGE, 0x31,                      /*     USAGE (Y) */
    _MULTIBYTE(_LOGICAL_MINIMUM), 0x00, 0x80,				 /* 	Logical Minimum (-32768) */
    _MULTIBYTE(_LOGICAL_MAXIMUM), 0xFF, 0x7F,				 /* 	Logical Maximum (32767) */
    _REPORT_SIZE, 0x10,						 /* 	Report Size (16), */
    _REPORT_COUNT, 0x02,						 /* 	Report Count (2), */
    _INPUT, (_DATA|_VARIABLE|_ABSOLUTE),						 /* 	Input (Data, Variable, Absolute) */

    /* Wheel */
    _USAGE, 0x38,                      /*     USAGE (Wheel) */
    _LOGICAL_MINIMUM, 0x81,                      /*     LOGICAL_MINIMUM (-127) */
    _LOGICAL_MAXIMUM, 0x7f,                      /*     LOGICAL_MAXIMUM (127) */
    _REPORT_SIZE, 0x08,                      /*     REPORT_SIZE (8) */
    _REPORT_COUNT, 0x01,                      /*     REPORT_COUNT (1) */
    _INPUT, (_DATA|_VARIABLE|_RELATIVE),

    /* End */
    _END_COLLECTION
};

AbsoluteMouse_::AbsoluteMouse_(void) {
    static HIDSubDescriptor node(_hidMultiReportDescriptorAbsoluteMouse, sizeof(_hidMultiReportDescriptorAbsoluteMouse));
    HID().AppendDescriptor(&node);
}


void AbsoluteMouse_::SendReport(void* data, int length) {
    HID().SendReport(HID_REPORTID_MOUSE_ABSOLUTE, data, length);
}


void AbsoluteMouse_::buttons(uint8_t b) {
    if (b != _buttons) {
        _buttons = b;
        moveTo(xAxis, yAxis, 0);
    }
}

int16_t AbsoluteMouse_::qadd16(int16_t base, int16_t increment) {
    // Separate between subtracting and adding
    if (increment < 0) {
        // Subtracting more would cause an undefined overflow
        if ((int16_t)0x8000 - increment > base)
            base = 0x8000;
        else
            base += increment;
    } else {
        // Adding more would cause an undefined overflow
        if ((int16_t)0x7FFF - increment < base)
            base = 0x7FFF;
        else
            base += increment;
    }
    return base;
}

void AbsoluteMouse_::begin(void) {
    // release all buttons
    end();
}

void AbsoluteMouse_::end(void) {
    _buttons = 0;
    moveTo(xAxis, yAxis, 0);
}

void AbsoluteMouse_::click(uint8_t b) {
    _buttons = b;
    moveTo(xAxis, yAxis, 0);
    _buttons = 0;
    moveTo(xAxis, yAxis, 0);
}

void AbsoluteMouse_::moveTo(int x, int y, signed char wheel) {
    xAxis = x;
    yAxis = y;
    HID_MouseAbsoluteReport_Data_t report;
    report.buttons = _buttons;
    report.xAxis = x;
    report.yAxis = y;
    report.wheel = wheel;
    SendReport(&report, sizeof(report));
}

void AbsoluteMouse_::move(int x, int y, signed char wheel) {
    moveTo(qadd16(xAxis, x), qadd16(yAxis, y), wheel);
}

void AbsoluteMouse_::press(uint8_t b) {
    // press LEFT by default
    buttons(_buttons | b);
}

void AbsoluteMouse_::release(uint8_t b) {
    // release LEFT by default
    buttons(_buttons & ~b);
}

bool AbsoluteMouse_::isPressed(uint8_t b) {
    // check LEFT by default
    if ((b & _buttons) > 0)
        return true;
    return false;
}


AbsoluteMouse_ AbsoluteMouse;


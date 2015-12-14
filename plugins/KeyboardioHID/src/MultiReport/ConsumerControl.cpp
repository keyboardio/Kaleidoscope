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

#include "ConsumerControl.h"
#include "DescriptorPrimitives.h"

static const uint8_t _hidMultiReportDescriptorConsumer[] PROGMEM = {
    /* Consumer Control (Sound/Media keys) */
    _USAGE_PAGE, 0x0C,									/* usage page (consumer device) */
    _USAGE, 0x01, 								/* usage -- consumer control */
    _COLLECTION, _APPLICATION, 								/* collection (application) */
    _REPORT_ID, HID_REPORTID_CONSUMERCONTROL, 		/* report id */
    /* 4 Media Keys */
    _LOGICAL_MINIMUM, 0x00, 								/* logical minimum */
    _MULTIBYTE(_LOGICAL_MAXIMUM), 0xFF, 0x03, 							/* logical maximum (3ff) */
    _USAGE_MINIMUM, 0x00, 								/* usage minimum (0) */
    _MULTIBYTE(_USAGE_MAXIMUM), 0xFF, 0x03, 							/* usage maximum (3ff) */
    _REPORT_COUNT, 0x04, 								/* report count (4) */
    _REPORT_SIZE, 0x10, 								/* report size (16) */
    _INPUT, 0x00, 								/* input */
    _END_COLLECTION /* end collection */
};

ConsumerControl_::ConsumerControl_(void) {
    static HIDSubDescriptor node(_hidMultiReportDescriptorConsumer, sizeof(_hidMultiReportDescriptorConsumer));
    HID().AppendDescriptor(&node);
}

void ConsumerControl_::begin(void) {
    // release all buttons
    end();
}

void ConsumerControl_::end(void) {
    memset(&_report, 0, sizeof(_report));
    SendReport(&_report, sizeof(_report));
}

void ConsumerControl_::write(uint16_t m) {
    press(m);
    release(m);
}

void ConsumerControl_::press(uint16_t m) {
    // search for a free spot
    for (uint8_t i = 0; i < sizeof(HID_ConsumerControlReport_Data_t) / 2; i++) {
        if (_report.keys[i] == 0x00) {
            _report.keys[i] = m;
            break;
        }
    }
    SendReport(&_report, sizeof(_report));
}

void ConsumerControl_::release(uint16_t m) {
    // search and release the keypress
    for (uint8_t i = 0; i < sizeof(HID_ConsumerControlReport_Data_t) / 2; i++) {
        if (_report.keys[i] == m) {
            _report.keys[i] = 0x00;
            // no break to delete multiple keys
        }
    }
    SendReport(&_report, sizeof(_report));
}

void ConsumerControl_::releaseAll(void) {
    end();
}

void ConsumerControl_::SendReport(void* data, int length) {
    HID().SendReport(HID_REPORTID_CONSUMERCONTROL, data, length);
}

ConsumerControl_ ConsumerControl;




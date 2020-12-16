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

#include "kaleidoscope/driver/hid/keyboardio/usb/MultiReport/ConsumerControl.h"
#include "kaleidoscope/driver/hid/keyboardio/usb/DescriptorPrimitives.h"

static const uint8_t _hidMultiReportDescriptorConsumer[] PROGMEM = {
  /* Consumer Control (Sound/Media keys) */
  D_USAGE_PAGE, 0x0C,									/* usage page (consumer device) */
  D_USAGE, 0x01, 								/* usage -- consumer control */
  D_COLLECTION, D_APPLICATION, 								/* collection (application) */
  D_REPORT_ID, HID_REPORTID_CONSUMERCONTROL, 		/* report id */
  /* 4 Media Keys */
  D_LOGICAL_MINIMUM, 0x00, 								/* logical minimum */
  D_MULTIBYTE(D_LOGICAL_MAXIMUM), 0xFF, 0x03, 							/* logical maximum (3ff) */
  D_USAGE_MINIMUM, 0x00, 								/* usage minimum (0) */
  D_MULTIBYTE(D_USAGE_MAXIMUM), 0xFF, 0x03, 							/* usage maximum (3ff) */
  D_REPORT_COUNT, 0x04, 								/* report count (4) */
  D_REPORT_SIZE, 0x10, 								/* report size (16) */
  D_INPUT, 0x00, 								/* input */
  D_END_COLLECTION /* end collection */
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
  sendReport();
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
}

void ConsumerControl_::release(uint16_t m) {
  // search and release the keypress
  for (uint8_t i = 0; i < sizeof(HID_ConsumerControlReport_Data_t) / 2; i++) {
    if (_report.keys[i] == m) {
      _report.keys[i] = 0x00;
      // no break to delete multiple keys
    }
  }
}

void ConsumerControl_::releaseAll(void) {
  memset(&_report, 0, sizeof(_report));
}

void ConsumerControl_::sendReportUnchecked(void) {
  HID().SendReport(HID_REPORTID_CONSUMERCONTROL, &_report, sizeof(_report));
}

void ConsumerControl_::sendReport(void) {
  // If the last report is different than the current report, then we need to send a report.
  // We guard sendReport like this so that calling code doesn't end up spamming the host with empty reports
  // if sendReport is called in a tight loop.

  // if the previous report is the same, return early without a new report.
  if (memcmp(&_lastReport, &_report, sizeof(_report)) == 0)
    return;

  sendReportUnchecked();
  memcpy(&_lastReport, &_report, sizeof(_report));
}

ConsumerControl_ ConsumerControl;

/*
Copyright (c) 2014-2015 NicoHood
Copyright (c) 2015-2024 Keyboard.io, Inc

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

static const uint8_t consumer_control_hid_descriptor_[] PROGMEM = {
  DESCRIPTOR_CONSUMER_CONTROL(HID_REPORT_ID(HID_REPORTID_CONSUMERCONTROL)),
};

ConsumerControl_::ConsumerControl_() {
  static HIDSubDescriptor node(consumer_control_hid_descriptor_,
                               sizeof(consumer_control_hid_descriptor_));
  HID().AppendDescriptor(&node);
}

void ConsumerControl_::begin() {
}

void ConsumerControl_::end() {
  memset(&report_, 0, sizeof(report_));
  sendReport();
}

void ConsumerControl_::write(uint16_t m) {
  press(m);
  release(m);
}

void ConsumerControl_::press(uint16_t m) {
  // search for a free spot
  for (uint8_t i = 0; i < sizeof(HID_ConsumerControlReport_Data_t) / 2; i++) {
    if (report_.keys[i] == 0x00) {
      report_.keys[i] = m;
      break;
    }
  }
}

void ConsumerControl_::release(uint16_t m) {
  // search and release the keypress
  for (uint8_t i = 0; i < sizeof(HID_ConsumerControlReport_Data_t) / 2; i++) {
    if (report_.keys[i] == m) {
      report_.keys[i] = 0x00;
      // no break to delete multiple keys
    }
  }
}

void ConsumerControl_::releaseAll() {
  memset(&report_, 0, sizeof(report_));
}

void ConsumerControl_::sendReportUnchecked() {
  HID().SendReport(HID_REPORTID_CONSUMERCONTROL, &report_, sizeof(report_));
}

void ConsumerControl_::sendReport() {
  // If the last report is different than the current report, then we need to
  // send a report.  We guard sendReport like this so that calling code doesn't
  // end up spamming the host with empty reports if sendReport is called in a
  // tight loop.

  // if the previous report is the same, return early without a new report.
  if (memcmp(&last_report_, &report_, sizeof(report_)) == 0)
    return;

  sendReportUnchecked();
  memcpy(&last_report_, &report_, sizeof(report_));
}

ConsumerControl_ ConsumerControl;

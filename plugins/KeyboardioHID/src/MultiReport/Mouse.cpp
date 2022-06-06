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

#include "Mouse.h"
#include "DescriptorPrimitives.h"

static const uint8_t mouse_hid_descriptor_[] PROGMEM = {
  /*  Mouse relative */
  D_USAGE_PAGE, D_PAGE_GENERIC_DESKTOP,           // USAGE_PAGE (Generic Desktop)
  D_USAGE, D_USAGE_MOUSE,                         // USAGE (Mouse)
  D_COLLECTION, D_APPLICATION,                    // COLLECTION (Application)
  D_REPORT_ID, HID_REPORTID_MOUSE,                // REPORT_ID (Mouse)

  /* 8 Buttons */
  D_USAGE_PAGE, D_PAGE_BUTTON,                    // USAGE_PAGE (Button)
  D_USAGE_MINIMUM, 0x01,                          // USAGE_MINIMUM (Button 1)
  D_USAGE_MAXIMUM, 0x08,                          // USAGE_MAXIMUM (Button 8)
  D_LOGICAL_MINIMUM, 0x00,                        // LOGICAL_MINIMUM (0)
  D_LOGICAL_MAXIMUM, 0x01,                        // LOGICAL_MAXIMUM (1)
  D_REPORT_COUNT, 0x08,                           // REPORT_COUNT (8)
  D_REPORT_SIZE, 0x01,                            // REPORT_SIZE (1)
  D_INPUT, (D_DATA | D_VARIABLE | D_ABSOLUTE),    // INPUT (Data,Var,Abs)

  /* X, Y, Wheel */
  D_USAGE_PAGE, D_PAGE_GENERIC_DESKTOP,           // USAGE_PAGE (Generic Desktop)
  D_USAGE, 0x30,                                  // USAGE (X)
  D_USAGE, 0x31,                                  // USAGE (Y)
  D_USAGE, 0x38,                                  // USAGE (Wheel)
  D_LOGICAL_MINIMUM, 0x81,                        // LOGICAL_MINIMUM (-127)
  D_LOGICAL_MAXIMUM, 0x7f,                        // LOGICAL_MAXIMUM (127)
  D_REPORT_SIZE, 0x08,                            // REPORT_SIZE (8)
  D_REPORT_COUNT, 0x03,                           // REPORT_COUNT (3)
  D_INPUT, (D_DATA | D_VARIABLE | D_RELATIVE),    // INPUT (Data,Var,Rel)

  /* Horizontal wheel */
  D_USAGE_PAGE, D_PAGE_CONSUMER,                  // USAGE_PAGE (Consumer)
  D_PAGE_ORDINAL, 0x38, 0x02,                     // PAGE (AC Pan)
  D_LOGICAL_MINIMUM, 0x81,                        // LOGICAL_MINIMUM (-127)
  D_LOGICAL_MAXIMUM, 0x7f,                        // LOGICAL_MAXIMUM (127)
  D_REPORT_SIZE, 0x08,                            // REPORT_SIZE (8)
  D_REPORT_COUNT, 0x01,                           // REPORT_COUNT (1)
  D_INPUT, (D_DATA | D_VARIABLE | D_RELATIVE),    // INPUT (Data,Var,Rel)

  /* End */
  D_END_COLLECTION                                // END_COLLECTION
};

Mouse_::Mouse_() {
  static HIDSubDescriptor node(mouse_hid_descriptor_,
                               sizeof(mouse_hid_descriptor_));
  HID().AppendDescriptor(&node);
}

void Mouse_::begin() {
}

void Mouse_::end() {
  releaseAll();
  sendReport();
}

void Mouse_::click(uint8_t b) {
  // If one or more of the buttons to be clicked was already pressed, we need to
  // send a report to release it first, to guarantee that this will be a "click"
  // and not merely a release.
  if (report_.buttons & b) {
    release(b);
    sendReport();
  }
  // Next, send a report with the button(s) pressed:
  press(b);
  sendReport();
  // Finally, send the report with the button(s) released:
  release(b);
  sendReport();
}

void Mouse_::move(int8_t x, int8_t y, int8_t v_wheel, int8_t h_wheel) {
  report_.xAxis = x;
  report_.yAxis = y;
  report_.vWheel = v_wheel;
  report_.hWheel = h_wheel;
}

void Mouse_::releaseAll() {
  memset(&report_, 0, sizeof(report_));
}

void Mouse_::press(uint8_t b) {
  report_.buttons |= b;
}

void Mouse_::release(uint8_t b) {
  report_.buttons &= ~b;
}

bool Mouse_::isPressed(uint8_t b) {
  if ((b & report_.buttons) > 0)
    return true;
  return false;
}

void Mouse_::sendReportUnchecked() {
  HID().SendReport(HID_REPORTID_MOUSE, &report_, sizeof(report_));
}

void Mouse_::sendReport() {
  // If the button state has not changed, and neither the cursor nor the wheel
  // is being told to move, there is no need to send a report.  This check
  // prevents us from sending lots of no-op reports if the caller is in a loop
  // and not checking or buggy.
  if (report_.buttons != prev_report_buttons_ ||
      report_.xAxis != 0 || report_.yAxis != 0 ||
      report_.vWheel != 0 || report_.hWheel != 0) {
    sendReportUnchecked();
    prev_report_buttons_ = report_.buttons;
  }
}

Mouse_ Mouse;

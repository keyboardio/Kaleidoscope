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

#pragma once

MouseAPI::MouseAPI()
  : report_{0} {}

void MouseAPI::begin() {
}

void MouseAPI::end() {
  releaseAll();
  sendReport();
}

void MouseAPI::click(uint8_t b) {
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

void MouseAPI::move(int8_t x, int8_t y, int8_t v_wheel, int8_t h_wheel) {
  report_.xAxis  = x;
  report_.yAxis  = y;
  report_.vWheel = v_wheel;
  report_.hWheel = h_wheel;
}

void MouseAPI::releaseAll() {
  memset(&report_, 0, sizeof(report_));
}

void MouseAPI::press(uint8_t b) {
  report_.buttons |= b;
}

void MouseAPI::release(uint8_t b) {
  report_.buttons &= ~b;
}

bool MouseAPI::isPressed(uint8_t b) {
  if ((b & report_.buttons) > 0)
    return true;
  return false;
}

void MouseAPI::sendReport() {
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

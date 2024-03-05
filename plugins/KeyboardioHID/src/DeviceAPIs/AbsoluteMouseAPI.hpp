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

AbsoluteMouseAPI::AbsoluteMouseAPI()
  : x_axis_(0), y_axis_(0), buttons_(0) {}

void AbsoluteMouseAPI::buttons(uint8_t b) {
  if (b != buttons_) {
    buttons_ = b;
    moveTo(x_axis_, y_axis_, 0);
  }
}

int16_t AbsoluteMouseAPI::qadd16(int16_t base, int16_t increment) {
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

void AbsoluteMouseAPI::begin() {
}

void AbsoluteMouseAPI::end() {
  buttons_ = 0;
  moveTo(x_axis_, y_axis_, 0);
}

void AbsoluteMouseAPI::click(uint8_t b) {
  buttons_ = b;
  moveTo(x_axis_, y_axis_, 0);
  buttons_ = 0;
  moveTo(x_axis_, y_axis_, 0);
}

void AbsoluteMouseAPI::moveTo(uint16_t x, uint16_t y, signed char wheel) {
  x_axis_ = x;
  y_axis_ = y;
  HID_MouseAbsoluteReport_Data_t report;
  report.buttons = buttons_;
  report.xAxis   = x;
  report.yAxis   = y;
  report.wheel   = wheel;
  sendReport(&report, sizeof(report));
}

void AbsoluteMouseAPI::move(int x, int y, signed char wheel) {
  moveTo(qadd16(x_axis_, x), qadd16(y_axis_, y), wheel);
}

void AbsoluteMouseAPI::press(uint8_t b) {
  // press LEFT by default
  buttons(buttons_ | b);
}

void AbsoluteMouseAPI::release(uint8_t b) {
  // release LEFT by default
  buttons(buttons_ & ~b);
}

bool AbsoluteMouseAPI::isPressed(uint8_t b) {
  // check LEFT by default
  if ((b & buttons_) > 0)
    return true;
  return false;
}

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

#include "Gamepad.h"

static const uint8_t gamepad_hid_descriptor_[] PROGMEM = {
  DESCRIPTOR_GAMEPAD(HID_REPORT_ID(HID_REPORTID_GAMEPAD)),
};

Gamepad_::Gamepad_() {
  static HIDSubDescriptor node(gamepad_hid_descriptor_,
                               sizeof(gamepad_hid_descriptor_));
  HID().AppendDescriptor(&node);
}

void Gamepad_::begin() {
}

void Gamepad_::end() {
  memset(&report_, 0x00, sizeof(report_));
  sendReport(&report_, sizeof(report_));
}

void Gamepad_::write() {
  sendReport(&report_, sizeof(report_));
}

void Gamepad_::press(uint8_t b) {
  report_.buttons |= (uint32_t)1 << (b - 1);
}

void Gamepad_::release(uint8_t b) {
  report_.buttons &= ~((uint32_t)1 << (b - 1));
}

void Gamepad_::releaseAll() {
  memset(&report_, 0x00, sizeof(report_));
}

void Gamepad_::buttons(uint32_t b) {
  report_.buttons = b;
}

void Gamepad_::xAxis(int16_t a) {
  report_.xAxis = a;
}

void Gamepad_::yAxis(int16_t a) {
  report_.yAxis = a;
}

void Gamepad_::zAxis(int8_t a) {
  report_.zAxis = a;
}

void Gamepad_::rxAxis(int16_t a) {
  report_.rxAxis = a;
}

void Gamepad_::ryAxis(int16_t a) {
  report_.ryAxis = a;
}

void Gamepad_::rzAxis(int8_t a) {
  report_.rzAxis = a;
}

void Gamepad_::dPad1(int8_t d) {
  report_.dPad1 = d;
}

void Gamepad_::dPad2(int8_t d) {
  report_.dPad2 = d;
}

void Gamepad_::sendReport(void *data, int length) {
  HID().SendReport(HID_REPORTID_GAMEPAD, data, length);
}

Gamepad_ Gamepad;

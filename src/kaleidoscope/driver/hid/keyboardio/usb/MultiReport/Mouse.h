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

// Include guard
#pragma once

#include <Arduino.h>
#include "kaleidoscope/driver/hid/keyboardio/usb/HID_.h"
#include "kaleidoscope/driver/hid/keyboardio/usb/HID-Settings.h"
#include "kaleidoscope/driver/hid/keyboardio/usb/MouseButtons.h"

typedef union {
  // Mouse report: 8 buttons, position, wheel
  struct {
    uint8_t buttons;
    int8_t xAxis;
    int8_t yAxis;
    int8_t vWheel;
    int8_t hWheel;
  };
} HID_MouseReport_Data_t;


class Mouse_ {
 public:
  Mouse_();
  void begin();
  void end();
  // Note: the following `click()` method is unlike the `move()`, `press()`, and
  // `release()` methods, in that it doesn't merely modify the pending report,
  // but also calls `sendReport()` at least twice.
  void click(uint8_t b = MOUSE_LEFT);
  void move(int8_t x, int8_t y, int8_t v_wheel = 0, int8_t h_wheel = 0);
  void press(uint8_t b = MOUSE_LEFT);   // press LEFT by default
  void release(uint8_t b = MOUSE_LEFT); // release LEFT by default
  bool isPressed(uint8_t b = MOUSE_LEFT); // check LEFT by default

  /** getReport returns the current report.
   *
   * The current report is the one to be send next time sendReport() is called.
   *
   * @returns A copy of the report.
   */
  const HID_MouseReport_Data_t getReport() {
    return report_;
  }
  void sendReport();

  void releaseAll();

 protected:
  HID_MouseReport_Data_t report_;
  uint8_t prev_report_buttons_ = 0;

 private:
  void sendReportUnchecked();
};
extern Mouse_ Mouse;

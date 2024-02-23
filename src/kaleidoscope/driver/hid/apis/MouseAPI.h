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

// Include guard
#pragma once

#include <Arduino.h>
#include "kaleidoscope/driver/hid/HIDDefs.h"
#include "kaleidoscope/MouseButtons.h"

#define DESCRIPTOR_MOUSE(...)                          \
  /*  Mouse relative */                                \
  HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),              \
    HID_USAGE(HID_USAGE_DESKTOP_MOUSE),                \
    HID_COLLECTION(HID_COLLECTION_APPLICATION),        \
    HID_USAGE(HID_USAGE_DESKTOP_POINTER),              \
    HID_COLLECTION(HID_COLLECTION_PHYSICAL),           \
                                                       \
    /* Report ID, if any */                            \
    __VA_ARGS__                                        \
                                                       \
    /* 8 Buttons */                                    \
    HID_USAGE_PAGE(HID_USAGE_PAGE_BUTTON),             \
    HID_USAGE_MIN(1),                                  \
    HID_USAGE_MAX(8),                                  \
    HID_LOGICAL_MIN(0),                                \
    HID_LOGICAL_MAX(1),                                \
    HID_REPORT_SIZE(1),                                \
    HID_REPORT_COUNT(8),                               \
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
                                                       \
    /* X, Y, Wheel */                                  \
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),            \
    HID_USAGE(HID_USAGE_DESKTOP_X),                    \
    HID_USAGE(HID_USAGE_DESKTOP_Y),                    \
    HID_USAGE(HID_USAGE_DESKTOP_WHEEL),                \
    HID_LOGICAL_MIN(0x81),                             \
    HID_LOGICAL_MAX(0x7f),                             \
    HID_REPORT_SIZE(8),                                \
    HID_REPORT_COUNT(3),                               \
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_RELATIVE), \
                                                       \
    /* Horizontal wheel */                             \
    HID_USAGE_PAGE(HID_USAGE_PAGE_CONSUMER),           \
    HID_USAGE_N(0x0238, 2),                            \
    HID_REPORT_COUNT(1),                               \
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_RELATIVE), \
                                                       \
    /* End */                                          \
    HID_COLLECTION_END,                                \
    HID_COLLECTION_END

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


class MouseAPI {
 public:
  inline MouseAPI();
  inline void begin();
  inline void end();
  // Note: the following `click()` method is unlike the `move()`, `press()`, and
  // `release()` methods, in that it doesn't merely modify the pending report,
  // but also calls `sendReport()` at least twice.
  inline void click(uint8_t b = MOUSE_LEFT);
  inline void move(int8_t x, int8_t y, int8_t v_wheel = 0, int8_t h_wheel = 0);
  inline void press(uint8_t b = MOUSE_LEFT);      // press LEFT by default
  inline void release(uint8_t b = MOUSE_LEFT);    // release LEFT by default
  inline bool isPressed(uint8_t b = MOUSE_LEFT);  // check LEFT by default

  /** getReport returns the current report.
   *
   * The current report is the one to be send next time sendReport() is called.
   *
   * @returns A copy of the report.
   */
  const HID_MouseReport_Data_t getReport() {
    return report_;
  }
  inline void sendReport();

  inline void releaseAll();

 protected:
  HID_MouseReport_Data_t report_;
  uint8_t prev_report_buttons_ = 0;

  virtual void sendReportUnchecked() = 0;
};

#include "MouseAPI.hpp"

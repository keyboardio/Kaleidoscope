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

#define DESCRIPTOR_ABSOLUTE_MOUSE(...)                 \
  HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),              \
    HID_USAGE(HID_USAGE_DESKTOP_MOUSE),                \
    HID_COLLECTION(HID_COLLECTION_APPLICATION),        \
                                                       \
    /* Report ID, if any */                            \
    __VA_ARGS__                                        \
                                                       \
    HID_USAGE(HID_USAGE_DESKTOP_POINTER),              \
    HID_COLLECTION(HID_COLLECTION_PHYSICAL),           \
                                                       \
    /* Buttons */                                      \
    HID_USAGE_PAGE(HID_USAGE_PAGE_BUTTON),             \
    HID_USAGE_MIN(1),                                  \
    HID_USAGE_MAX(8),                                  \
    HID_LOGICAL_MIN(0),                                \
    HID_LOGICAL_MAX(1),                                \
    HID_REPORT_COUNT(8),                               \
    HID_REPORT_SIZE(1),                                \
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
                                                       \
    /* X, Y */                                         \
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),            \
    HID_USAGE(HID_USAGE_DESKTOP_X),                    \
    HID_USAGE(HID_USAGE_DESKTOP_Y),                    \
    HID_LOGICAL_MIN_N(0, 2),                           \
    HID_LOGICAL_MAX_N(0x7fff, 2),                      \
    HID_REPORT_SIZE(16),                               \
    HID_REPORT_COUNT(2),                               \
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
                                                       \
    /* Wheel [-127, 127] */                            \
    HID_USAGE(HID_USAGE_DESKTOP_WHEEL),                \
    HID_LOGICAL_MIN(0x81),                             \
    HID_LOGICAL_MAX(0x7f),                             \
    HID_REPORT_SIZE(8),                                \
    HID_REPORT_COUNT(1),                               \
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_RELATIVE), \
                                                       \
    HID_COLLECTION_END,                                \
    HID_COLLECTION_END

#pragma pack(push, 1)
typedef union {
  // Absolute mouse report: 8 buttons, 2 absolute axis, wheel
  struct {
    uint8_t buttons;
    uint16_t xAxis;
    uint16_t yAxis;
    int8_t wheel;
  };
} HID_MouseAbsoluteReport_Data_t;
#pragma pack(pop)

class AbsoluteMouseAPI {
 public:
  inline AbsoluteMouseAPI();
  inline void begin();
  inline void end();

  inline void click(uint8_t b = MOUSE_LEFT);
  inline void moveTo(uint16_t x, uint16_t y, int8_t wheel = 0);
  inline void move(int x, int y, int8_t wheel = 0);
  inline void press(uint8_t b = MOUSE_LEFT);
  inline void release(uint8_t b = MOUSE_LEFT);
  inline bool isPressed(uint8_t b = MOUSE_LEFT);

  // Sending is public in the base class for advanced users.
  virtual void sendReport(void *data, int length) {}

 protected:
  uint16_t x_axis_;
  uint16_t y_axis_;
  uint8_t buttons_;

  inline void buttons(uint8_t b);
  inline int16_t qadd16(int16_t base, int16_t increment);
};

#include "AbsoluteMouseAPI.hpp"

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

#define DESCRIPTOR_CONSUMER_CONTROL(...)            \
  HID_USAGE_PAGE(HID_USAGE_PAGE_CONSUMER),          \
                                                    \
    /* Consumer Control (Sound/Media keys) */       \
    HID_USAGE(HID_USAGE_CONSUMER_CONTROL),          \
    HID_COLLECTION(HID_COLLECTION_APPLICATION),     \
                                                    \
    /* Report ID, if any */                         \
    __VA_ARGS__                                     \
                                                    \
      /* 4 Media Keys */                            \
      HID_LOGICAL_MIN(0),                           \
    HID_LOGICAL_MAX_N(0x3ff, 2),                    \
    HID_USAGE_MIN(0),                               \
    HID_USAGE_MAX_N(0x3ff, 2),                      \
    HID_REPORT_COUNT(4),                            \
    HID_REPORT_SIZE(16),                            \
    HID_INPUT(HID_DATA | HID_ARRAY | HID_ABSOLUTE), \
    HID_COLLECTION_END

typedef union {
  // Every usable Consumer key possible, up to 4 keys presses possible
  uint16_t keys[4];
  struct {
    uint16_t key1;
    uint16_t key2;
    uint16_t key3;
    uint16_t key4;
  };
} HID_ConsumerControlReport_Data_t;


class ConsumerControlAPI {
 public:
  inline ConsumerControlAPI();
  inline void begin();
  inline void end();
  inline void write(uint16_t m);
  inline void press(uint16_t m);
  inline void release(uint16_t m);
  inline void releaseAll();

  inline void sendReport();

 protected:
  virtual void sendReportUnchecked() = 0;

  HID_ConsumerControlReport_Data_t report_;
  HID_ConsumerControlReport_Data_t last_report_;
};

#include "ConsumerControlAPI.hpp"

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

#include "kaleidoscope/driver/hid/HIDDefs.h"

#define DESCRIPTOR_SYSTEM_CONTROL(...)                  \
  /* TODO(anyone) limit to system keys only? */         \
  /* System Control (Power Down, Sleep, Wakeup, ...) */ \
  HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),               \
    HID_USAGE(HID_USAGE_DESKTOP_SYSTEM_CONTROL),        \
    HID_COLLECTION(HID_COLLECTION_APPLICATION),         \
                                                        \
    /* Report ID (if any) */                            \
    __VA_ARGS__                                         \
                                                        \
      HID_LOGICAL_MIN(0),                               \
    HID_LOGICAL_MAX_N(255, 2),                          \
    HID_USAGE_MIN(0),                                   \
    HID_USAGE_MAX(255),                                 \
    HID_REPORT_COUNT(1),                                \
    HID_REPORT_SIZE(8),                                 \
    HID_INPUT(HID_DATA | HID_ARRAY | HID_ABSOLUTE),     \
                                                        \
    HID_COLLECTION_END

typedef union {
  // Every usable system control key possible
  uint8_t key;
} HID_SystemControlReport_Data_t;


class SystemControlAPI {
 public:
  inline SystemControlAPI();

  inline void begin();
  inline void end();
  inline void write(uint8_t s);
  inline void press(uint8_t s);
  inline void release();
  inline void releaseAll();

 protected:
  virtual void sendReport(void *data, int length) = 0;
  virtual bool wakeupHost(uint8_t s)              = 0;
};

#include "SystemControlAPI.hpp"

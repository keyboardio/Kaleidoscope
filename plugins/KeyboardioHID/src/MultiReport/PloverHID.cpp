/*
Copyright (c) 2014-2015 NicoHood
Copyright (c) 2015-2025 Keyboard.io, inc

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

#include "PloverHID.h"

static const uint8_t plover_hid_descriptor_[] PROGMEM = {
  DESCRIPTOR_PLOVER_HID(HID_REPORT_ID(HID_REPORTID_PLOVER_HID)),
};

PloverHID_::PloverHID_() {
  static HIDSubDescriptor node(plover_hid_descriptor_,
                               sizeof(plover_hid_descriptor_));
  HID().AppendDescriptor(&node);
}

void PloverHID_::sendReport(void *data, int length) {
  HID().SendReport(HID_REPORTID_PLOVER_HID, data, length);
}

PloverHID_ PloverHID;

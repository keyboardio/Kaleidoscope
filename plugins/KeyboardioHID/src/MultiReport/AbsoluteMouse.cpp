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

#include "AbsoluteMouse.h"
#include "DescriptorPrimitives.h"

static const uint8_t absolute_mouse_hid_descriptor_[] PROGMEM = {
  /*  Mouse absolute */
  D_USAGE_PAGE,
  D_PAGE_GENERIC_DESKTOP, /* USAGE_PAGE (Generic Desktop)   54 */
  D_USAGE,
  D_USAGE_MOUSE, /* USAGE (Mouse) */
  D_COLLECTION,
  D_APPLICATION, /* COLLECTION (Application) */
  D_USAGE,
  D_USAGE_POINTER, /* USAGE (Pointer) */
  D_COLLECTION,
  D_PHYSICAL, /* COLLECTION (Physical)*/
  D_REPORT_ID,
  HID_REPORTID_MOUSE_ABSOLUTE, /*  REPORT_ID */

  DESCRIPTOR_ABS_MOUSE_BUTTONS
    DESCRIPTOR_ABS_MOUSE_XY
      DESCRIPTOR_ABS_MOUSE_WHEEL

        D_END_COLLECTION,
  D_END_COLLECTION /* End */
};

AbsoluteMouse_::AbsoluteMouse_() {
  static HIDSubDescriptor node(absolute_mouse_hid_descriptor_,
                               sizeof(absolute_mouse_hid_descriptor_));
  HID().AppendDescriptor(&node);
}


void AbsoluteMouse_::sendReport(void *data, int length) {
  HID().SendReport(HID_REPORTID_MOUSE_ABSOLUTE, data, length);
}

AbsoluteMouse_ AbsoluteMouse;

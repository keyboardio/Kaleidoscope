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

#include "SystemControl.h"

static const uint8_t system_control_hid_descriptor_[] PROGMEM = {
  DESCRIPTOR_SYSTEM_CONTROL(HID_REPORT_ID(HID_REPORTID_SYSTEMCONTROL)),
};

SystemControl_::SystemControl_() {
  static HIDSubDescriptor node(system_control_hid_descriptor_,
                               sizeof(system_control_hid_descriptor_));
  HID().AppendDescriptor(&node);
}


bool SystemControl_::wakeupHost(uint8_t s) {
  if (s == HID_SYSTEM_WAKE_UP) {
#ifdef __AVR__
    USBDevice.wakeupHost();
#endif
#ifdef ARDUINO_ARCH_SAMD
    // This is USBDevice_SAMD21G18x.wakeupHost(). But we can't include that
    // header, because it redefines a few symbols, and causes linking
    // errors. So we simply reimplement the same thing here.
    USB->DEVICE.CTRLB.bit.UPRSM = 1;
#endif
    return true;
  }
  return false;
}


void SystemControl_::sendReport(void *data, int length) {
  HID().SendReport(HID_REPORTID_SYSTEMCONTROL, data, length);
}

SystemControl_ SystemControl;

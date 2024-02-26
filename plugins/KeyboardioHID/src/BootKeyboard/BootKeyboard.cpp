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

#include "BootKeyboard.h"
#include "HIDReportObserver.h"
#include "HID-Settings.h"

static const uint8_t boot_keyboard_hid_descriptor_[] PROGMEM = {
  DESCRIPTOR_BOOT_KEYBOARD(),
};

static const uint8_t hybrid_keyboard_hid_descriptor_[] PROGMEM = {
  DESCRIPTOR_HYBRID_KEYBOARD(),
};

BootKeyboard_::BootKeyboard_(uint8_t bootkb_only_)
  : BootKeyboardAPI(bootkb_only_) {

  itfProtocol = HID_ITF_PROTOCOL_KEYBOARD;
  setBootOnly(bootkb_only);
  plug();
}

uint8_t BootKeyboard_::getLeds() {
  return outReport[0];
}

/*
 * Set whether to send only Boot Protocol regardless of whether Boot
 * Protocol has been requested. Used by USBQuirks to toggle protocol
 * modes.
 *
 * This is only really safe to call after detaching from USB, because
 * otherwise, the report format might get out of sync with what the host
 * expects, and the host probably won't see the new Report Descriptor
 * until after a re-attach.
 */

void BootKeyboard_::setReportDescriptor(uint8_t bootkb_only) {
  if (bootkb_only) {
    reportDesc     = boot_keyboard_hid_descriptor_;
    descriptorSize = sizeof(boot_keyboard_hid_descriptor_);
  } else {
    reportDesc     = hybrid_keyboard_hid_descriptor_;
    descriptorSize = sizeof(hybrid_keyboard_hid_descriptor_);
  }
}

int BootKeyboard_::SendHIDReport(const void *data, int len) {
  int returnCode = HIDD::SendReport(0, data, len);
  HIDReportObserver::observeReport(HID_REPORTID_KEYBOARD, data, len, returnCode);
}

/*
 * Hook function to reset any needed state after a USB reset.
 *
 * Right now, it sets the protocol back to the default (report protocol), as
 * required by the HID specification.
 */
void BootKeyboard_::onUSBReset() {
  protocol = HID_PROTOCOL_REPORT;
}

__attribute__((weak))
BootKeyboard_ &
BootKeyboard() {
  static BootKeyboard_ obj;
  return obj;
}

// -*- mode: c++ -*-
/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2024  Keyboard.io, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef USE_TINYUSB

#include "kaleidoscope/driver/hid/tinyusb/Keyboard.h"

namespace kaleidoscope {
namespace driver {
namespace hid {
namespace tinyusb {

static uint8_t BootKeyboardDesc[] = {
  DESCRIPTOR_BOOT_KEYBOARD(),
};

static uint8_t HybridKeyboardDesc[] = {
  DESCRIPTOR_HYBRID_KEYBOARD(),
};

uint8_t BootKeyboard_::leds = 0;

void boot_keyboard_set_report_cb(
  uint8_t report_id,
  hid_report_type_t report_type,
  uint8_t const *buffer,
  uint16_t bufsize) {
  if (report_id != 0) {
    return;
  }
  if (report_type != HID_REPORT_TYPE_OUTPUT) {
    return;
  }
  if (bufsize != 1) {
    return;
  }
  BootKeyboard_::leds = buffer[0];
}

BootKeyboard_::BootKeyboard_(uint8_t bootkb_only_)
  : BootKeyboardAPI(bootkb_only_),
    HIDD(bootkb_only_ ? BootKeyboardDesc : HybridKeyboardDesc,
         bootkb_only_ ? sizeof(BootKeyboardDesc) : sizeof(HybridKeyboardDesc),
         HID_ITF_PROTOCOL_KEYBOARD,
         1) {
  setReportCallback(NULL, boot_keyboard_set_report_cb);
}

void BootKeyboard_::setReportDescriptor(uint8_t bootkb_only) {
  if (bootkb_only) {
    Adafruit_USBD_HID::setReportDescriptor(BootKeyboardDesc, sizeof(BootKeyboardDesc));
  } else {
    Adafruit_USBD_HID::setReportDescriptor(HybridKeyboardDesc, sizeof(HybridKeyboardDesc));
  }
}

BootKeyboard_ &BootKeyboard() {
  static BootKeyboard_ obj;
  return obj;
}

}  // namespace tinyusb
}  // namespace hid
}  // namespace driver
}  // namespace kaleidoscope
#endif

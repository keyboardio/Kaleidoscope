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

#include <stdint.h>  // for uint8_t, int8_t, uint16_t

#include "Adafruit_TinyUSB.h"
#include "MultiReport.h"

namespace kaleidoscope {
namespace driver {
namespace hid {
namespace tinyusb {

static const uint8_t TUSBMultiReportDesc[] = {
  DESCRIPTOR_CONSUMER_CONTROL(HID_REPORT_ID(RID_CONSUMER_CONTROL)),
  DESCRIPTOR_MOUSE(HID_REPORT_ID(RID_MOUSE)),
  DESCRIPTOR_SYSTEM_CONTROL(HID_REPORT_ID(RID_SYSTEM_CONTROL)),
};


TUSBMultiReport_::TUSBMultiReport_()
  : HIDD(TUSBMultiReportDesc, sizeof(TUSBMultiReportDesc), HID_ITF_PROTOCOL_NONE, 1) {
}


TUSBMultiReport_ &TUSBMultiReport() {
  static TUSBMultiReport_ obj;
  return obj;
}

}  // namespace tinyusb
}  // namespace hid
}  // namespace driver
}  // namespace kaleidoscope

#endif /* USE_TINYUSB */

// -*- mode: c++ -*-
/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2019  Keyboard.io, Inc.
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

#pragma once

#ifdef USE_TINYUSB

#include <stdint.h>  // for uint8_t, int8_t, uint16_t

#include "Adafruit_TinyUSB.h"
#include "HIDD.h"
#include "kaleidoscope/driver/hid/apis/AbsoluteMouseAPI.h"  // for AbsoluteMouse, AbsoluteMouseProps

// IWYU pragma: no_include "DeviceAPIs/AbsoluteMouseAPI.hpp"

namespace kaleidoscope {
namespace driver {
namespace hid {
namespace tinyusb {

static const uint8_t AbsoluteMouseDesc[] = {
  DESCRIPTOR_ABSOLUTE_MOUSE(),
};

class TUSBAbsoluteMouse : public AbsoluteMouseAPI, public HIDD {
 public:
  TUSBAbsoluteMouse()
    : HIDD(AbsoluteMouseDesc, sizeof(AbsoluteMouseDesc), HID_ITF_PROTOCOL_NONE, 1) {}
  void begin() {
    (void)HIDD::begin();
    AbsoluteMouseAPI::begin();
  }
  void sendReport(void *data, int length) override {
    (void)HIDD::sendReport(0, data, length);
  }
};

struct AbsoluteMouseProps : public base::AbsoluteMouseProps {
  typedef TUSBAbsoluteMouse AbsoluteMouse;
};

template<typename _Props>
class AbsoluteMouse : public base::AbsoluteMouse<_Props> {};

}  // namespace tinyusb
}  // namespace hid
}  // namespace driver
}  // namespace kaleidoscope

#endif /* USE_TINYUSB */

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

class TUSBAbsoluteMouse_ : public AbsoluteMouseAPI, public HIDD {
 public:
  TUSBAbsoluteMouse_();
  void begin() {
    (void)HIDD::begin();
    AbsoluteMouseAPI::begin();
  }
  void sendReport(void *data, int length) override {
    (void)HIDD::sendReport(0, data, length);
  }
};

extern TUSBAbsoluteMouse_ &TUSBAbsoluteMouse();

class AbsoluteMouseWrapper {
 public:
  AbsoluteMouseWrapper() {}

  void begin() {
    TUSBAbsoluteMouse().begin();
  }
  void move(int8_t x, int8_t y, int8_t wheel) {
    TUSBAbsoluteMouse().move(x, y, wheel);
  }
  void moveTo(uint16_t x, uint16_t y, uint8_t wheel) {
    TUSBAbsoluteMouse().moveTo(x, y, wheel);
  }

  void click(uint8_t buttons) {
    TUSBAbsoluteMouse().click(buttons);
  }
  void press(uint8_t buttons) {
    TUSBAbsoluteMouse().press(buttons);
  }
  void release(uint8_t buttons) {
    TUSBAbsoluteMouse().release(buttons);
  }
};

struct AbsoluteMouseProps : public base::AbsoluteMouseProps {
  typedef AbsoluteMouseWrapper AbsoluteMouse;
};

template<typename _Props>
class AbsoluteMouse : public base::AbsoluteMouse<_Props> {};

}  // namespace tinyusb
}  // namespace hid
}  // namespace driver
}  // namespace kaleidoscope

#endif /* USE_TINYUSB */

/* -*- mode: c++ -*-
 * kaleidoscope::driver::bootloader::samd::Bossac -- Driver for the SAMD bootloader
 * Copyright (C) 2019  Keyboard.io, Inc
 * Copyright (C) 2019  Dygma, Inc
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

#ifdef ARDUINO_ARCH_SAMD

#include <Arduino.h>

#include "kaleidoscope/driver/bootloader/Base.h"

namespace kaleidoscope {
namespace driver {
namespace bootloader {
namespace samd {

class Bossac : public kaleidoscope::driver::bootloader::Base {
 public:
  static void rebootBootloader() {
    __attribute__((__aligned__(4))) UsbDeviceDescriptor EP[USB_EPT_NUM];

    USB->DEVICE.CTRLA.bit.SWRST = 1;
    memset(EP, 0, sizeof(EP));
    while (USB->DEVICE.SYNCBUSY.bit.SWRST) {}
    USB->DEVICE.DESCADD.reg = (uint32_t)(&EP);
  }
};

}  // namespace samd
}  // namespace bootloader
}  // namespace driver
}  // namespace kaleidoscope

#endif

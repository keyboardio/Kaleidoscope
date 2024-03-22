/* -*- mode: c++ -*-
 * Copyright (C) 2019-2024  Keyboard.io, Inc
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

/* Shared "MCU" definition for platforms with TinyUSB device driver */

#pragma once

#include <Arduino.h>  // NVIC_Reset
#include "Adafruit_TinyUSB.h"
#include "kaleidoscope/driver/mcu/Base.h"  // for Base, BaseProps

namespace kaleidoscope {
namespace driver {
namespace mcu {

struct TinyUSBProps : public kaleidoscope::driver::mcu::BaseProps {
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
template<typename _Props>
class TinyUSB : public kaleidoscope::driver::mcu::Base<_Props> {
 public:
  void detachFromHost() {
    TinyUSBDevice.detach();
  }
  void attachToHost() {
    TinyUSBDevice.attach();
  }
  bool USBConfigured() {
    /* "mounted" is how TinyUSB spells "configured" */
    return TinyUSBDevice.mounted();
  }
  bool pollUSBReset() {
    return false;
  }
  void setUSBResetHook(void (*hook)()) {
    (void)hook;
  }


  void setup() {
  }
};
#else
template<typename _Props>
class TinyUSB : public kaleidoscope::driver::mcu::Base<_Props> {};
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

}  // namespace mcu
}  // namespace driver
}  // namespace kaleidoscope

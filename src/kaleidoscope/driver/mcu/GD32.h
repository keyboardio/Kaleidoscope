/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2019-2025 Keyboard.io, inc.
 *
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * Additional Permissions:
 * As an additional permission under Section 7 of the GNU General Public
 * License Version 3, you may link this software against a Vendor-provided
 * Hardware Specific Software Module under the terms of the MCU Vendor
 * Firmware Library Additional Permission Version 1.0.
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

#include <Arduino.h>                       // NVIC_Reset
#include <USBCore.h>                       // For connect, disconnect, USBCore
#include "kaleidoscope/driver/mcu/Base.h"  // for Base, BaseProps

namespace kaleidoscope {
namespace driver {
namespace mcu {

struct GD32Props : public kaleidoscope::driver::mcu::BaseProps {
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
template<typename _Props>
class GD32 : public kaleidoscope::driver::mcu::Base<_Props> {
 public:
  void detachFromHost() {
    USBCore().disconnect();
  }
  void attachToHost() {
    USBCore().connect();
  }
  bool USBConfigured() {
    return USBCore().configured();
  }
  bool pollUSBReset() {
    return false;
  }
  void setUSBResetHook(void (*hook)()) {
    USBCore().setResetHook(hook);
  }


  void setup() {
  }
};
#else
template<typename _Props>
class GD32 : public kaleidoscope::driver::mcu::Base<_Props> {};
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

}  // namespace mcu
}  // namespace driver
}  // namespace kaleidoscope

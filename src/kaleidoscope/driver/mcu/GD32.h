/* -*- mode: c++ -*-
 * driver::MCU::GD32 -- GD32 MCU driver for Kaleidoscope, initially targeting the GD32F303 series
 * Copyright (C) 2019, 2020  Keyboard.io, Inc
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

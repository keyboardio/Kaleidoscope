/* -*- mode: c++ -*-
 * driver::mcu::SAMD -- SAMD MCU driver class for Kaleidoscope
 * Copyright (C) 2019, 2020  Keyboard.io, Inc
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

#include "kaleidoscope/driver/mcu/Base.h"

namespace kaleidoscope {
namespace driver {
namespace mcu {

class SAMD : public kaleidoscope::driver::mcu::Base<BaseProps> {
 public:
  void detachFromHost() {
    USBDevice.detach();
  }
  void attachToHost() {
    USBDevice.attach();
  }
  bool USBConfigured() {
    return USBDevice.configured();
  }
};

}  // namespace mcu
}  // namespace driver
}  // namespace kaleidoscope

#endif

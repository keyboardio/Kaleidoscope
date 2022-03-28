/* -*- mode: c++ -*-
 * kaleidoscope::driver::bootloader::gd32::base -- GD32 Base Bootloader Driver
 * Copyright (C) 2021  Keyboard.io, Inc
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

#include <Arduino.h>  // IWYU pragma: keep

#include "kaleidoscope/driver/bootloader/Base.h"

namespace kaleidoscope {
namespace driver {
namespace bootloader {
namespace gd32 {

class Base : public kaleidoscope::driver::bootloader::Base {
 public:
  static void rebootBootloader() {
    NVIC_SystemReset();
  }
};

}  // namespace gd32
}  // namespace bootloader
}  // namespace driver
}  // namespace kaleidoscope

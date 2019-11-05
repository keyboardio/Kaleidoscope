/* -*- mode: c++ -*-
 * kaleidoscope::driver::bootloader::avr::FLIP -- Driver for the Atmel FLIP bootloader
 * Copyright (C) 2019  Keyboard.io, Inc
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

#include <avr/wdt.h>

#ifndef KALEIDOSCOPE_BOOTLOADER_FLIP_WORKAROUND
#error To use the FLIP bootloader driver, KALEIDOSCOPE_BOOTLOADER_FLIP_WORKAROUND *must* be defined prior to including this header!
#endif

#include "kaleidoscope/driver/bootloader/Base.h"

namespace kaleidoscope {
namespace driver {
namespace bootloader {
namespace avr {

class FLIP : public kaleidoscope::driver::bootloader::Base {
 public:
  static void rebootBootloader();
};

}
}
}
}

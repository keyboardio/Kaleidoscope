/* -*- mode: c++ -*-
 * kaleidoscope::driver::bootloader::None -- Dummy Bootloader driver
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
#include "kaleidoscope/driver/BaseBootloader.h"

namespace kaleidoscope {
namespace driver {
namespace bootloader {

/*
 * The purpose of this class is to serve as a default for the base
 * `DeviceDescription` class, with a name more descriptive than
 * `BaseBootloader`. In practice, one shouldn't use it, and should override the
 * bootloader in the device description.
 */
class None : public kaleidoscope::driver::BaseBootloader {};

}
}
}

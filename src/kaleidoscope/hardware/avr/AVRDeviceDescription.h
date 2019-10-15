/* -*- mode: c++ -*-
 * hardware::avr::AVRDeviceDescription -- Generic AVR device description base class
 * Copyright (C) 2019  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "kaleidoscope/driver/mcu/ATMega32U4.h"
#include "kaleidoscope/driver/storage/AVREEPROM.h"

#include "kaleidoscope/DeviceDescription.h"

namespace kaleidoscope {
namespace hardware {
namespace avr {

struct AVRDeviceDescription : kaleidoscope::DeviceDescription {
  typedef kaleidoscope::driver::mcu::ATMega32U4 MCU;
  typedef kaleidoscope::driver::storage::AVREEPROMStorageDescription StorageDescription;
  typedef kaleidoscope::driver::storage::AVREEPROM<StorageDescription> Storage;
};

}
}
}

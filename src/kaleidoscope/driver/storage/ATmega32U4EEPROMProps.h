/* -*- mode: c++ -*-
 * kaleidoscope::driver::storage::ATmega32U4StorageProps -- Storage driver props for ATmega32U4
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

#include <stdint.h>  // for uint16_t

#include "kaleidoscope/driver/storage/AVREEPROM.h"  // for AVREEPROMProps

namespace kaleidoscope {
namespace driver {
namespace storage {

struct ATmega32U4EEPROMProps : kaleidoscope::driver::storage::AVREEPROMProps {
  static constexpr uint16_t length = 1024;
};

}  // namespace storage
}  // namespace driver
}  // namespace kaleidoscope

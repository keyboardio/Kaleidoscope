/* -*- mode: c++ -*-
 * kaleidoscope::driver::keyscanner::base -- Keyscanner base class
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

#include "kaleidoscope/driver/keyscanner/Base.h"
#include "kaleidoscope/device/device.h"

namespace kaleidoscope {
namespace driver {
namespace keyscanner {

template<>
void Base<kaleidoscope::Device::Props::KeyScannerProps>::handleKeyswitchEvent(Key mappedKey, kaleidoscope::Device::Props::KeyScannerProps::KeyAddr key_addr, uint8_t keyState) {
  ::handleKeyswitchEvent(mappedKey, key_addr, keyState);
}
} // namespace keyscanner
} // namespace driver
} // namespace kaleidoscope

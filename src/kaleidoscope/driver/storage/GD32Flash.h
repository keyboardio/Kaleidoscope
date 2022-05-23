/* -*- mode: c++ -*-
 * kaleidoscope::driver::storage::GD32Flash -- Storage driver with GD32 flash backend
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

#if defined(ARDUINO_ARCH_GD32) || defined(KALEIDOSCOPE_VIRTUAL_BUILD)

#include <FlashAsEEPROM.h>
#include <FlashStorage.h>

#include "kaleidoscope/driver/storage/Base.h"

namespace kaleidoscope {
namespace driver {
namespace storage {

struct GD32FlashProps : kaleidoscope::driver::storage::BaseProps {
  static constexpr uint16_t length = 16384;
};

template<typename _StorageProps>
class GD32Flash : public EEPROMClass<_StorageProps::length> {
 public:
  void setup() {
    EEPROMClass<_StorageProps::length>::begin();
  }

  bool isSliceUninitialized(uint16_t offset, uint16_t size) {
    for (uint16_t o = offset; o < offset + size; o++) {
      if (this->read(o) != _StorageProps::uninitialized_byte)
        return false;
    }
    return true;
  }
};

}  // namespace storage
}  // namespace driver
}  // namespace kaleidoscope

#endif

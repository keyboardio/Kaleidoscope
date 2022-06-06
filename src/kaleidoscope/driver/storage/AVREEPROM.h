/* -*- mode: c++ -*-
 * kaleidoscope::driver::storage::AVREEPROM -- Storage driver with AVR EEPROM backend
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

#include <stdint.h>  // for uint16_t, uint8_t
#if defined(__AVR__) || defined(KALEIDOSCOPE_VIRTUAL_BUILD)

#include <EEPROM.h>  // for EEPROM, EEPROMClass

#include "kaleidoscope/driver/storage/Base.h"  // for Base, BaseProps

namespace kaleidoscope {
namespace driver {
namespace storage {

struct AVREEPROMProps : kaleidoscope::driver::storage::BaseProps {
  static constexpr uint16_t length = 0;
};

template<typename _StorageProps>
class AVREEPROM : public kaleidoscope::driver::storage::Base<_StorageProps> {
 public:
  template<typename T>
  static T &get(uint16_t offset, T &t) {
    return EEPROM.get(offset, t);
  }

  template<typename T>
  static const T &put(uint16_t offset, T &t) {
    return EEPROM.put(offset, t);
  }

  uint8_t read(int idx) {
    return EEPROM.read(idx);
  }

  void write(int idx, uint8_t val) {
    EEPROM.write(idx, val);
  }

  void update(int idx, uint8_t val) {
    EEPROM.update(idx, val);
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

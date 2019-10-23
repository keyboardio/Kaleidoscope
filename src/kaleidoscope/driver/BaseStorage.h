/* -*- mode: c++ -*-
 * kaleidoscope::driver::BaseStorage -- Base Storage driver class
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

namespace kaleidoscope {
namespace driver {

template <typename _StorageDescription>
class BaseStorage {
 public:
  template<typename T>
  static T& get(uint16_t offset, T& t) {
    return t;
  }

  template<typename T>
  static const T& put(uint16_t offset, T& t) {
    return t;
  }

  uint8_t read(int idx) {
    return 0;
  }

  void write(int idx, uint8_t val) {}

  void update(int idx, uint8_t val) {}

  const uint16_t length() {
    return _StorageDescription::length;
  }

  void commit() {}
};

}
}

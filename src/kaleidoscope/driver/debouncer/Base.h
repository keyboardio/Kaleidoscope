/* -*- mode: c++ -*-
 * kaleidoscope::driver::debouncer::Base -- Debouncer driver base class for Kaleidoscope
 * Copyright (C) 2020  Keyboard.io, Inc
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

#include <Arduino.h>

namespace kaleidoscope {
namespace driver {
namespace debouncer {

template <uint8_t _rows, typename RowState>
class Base {
 public:
  Base() {}

  RowState debounce(RowState sample, uint8_t row) {
    return 0;
  }

  RowState getRowState(uint8_t row) {
    return 0;
  }
};

}
}
}

/* -*- mode: c++ -*-
 * kaleidoscope::driver::debouncer::None -- No-debouncer driver class for Kaleidoscope
 * Copyright (C) 2018-2020  Keyboard.io, Inc
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

#include "kaleidoscope/driver/debouncer/Base.h"

namespace kaleidoscope {
namespace driver {
namespace debouncer {

template <uint8_t _rows, typename RowState>
class None: public kaleidoscope::driver::debouncer::Base<_rows, RowState> {
 protected:
  struct debounce_t {
    RowState debounced_state;
  };

  debounce_t debounce_state_[_rows];
 public:
  RowState debounce(RowState sample, uint8_t row) {
    RowState changes = sample ^ debounce_state_[row].debounced_state;
    if (changes)
      debounce_state_[row].debounced_state = changes;
    return changes;
  }

  RowState getRowState(uint8_t row) {
    return debounce_state_[row].debounced_state;
  }
};

}
}
}

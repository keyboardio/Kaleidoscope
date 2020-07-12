/* -*- mode: c++ -*-
 * kaleidoscope::driver::debouncer::Counter -- Counter based debouncer driver class for Kaleidoscope
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
class Counter: public kaleidoscope::driver::debouncer::Base<_rows, RowState> {
 protected:
  /*
    each of these variables are storing the state for a row of keys

    so for key 0, the counter is represented by db0[0] and db1[0]
    and the state in debounced_state[0].
  */
  struct debounce_t {
    RowState db0;    // counter bit 0
    RowState db1;    // counter bit 1
    RowState debounced_state;  // debounced state
  };

  debounce_t debounce_state_[_rows];
 public:
  RowState debounce(RowState sample, uint8_t row) {
    RowState delta, changes;

    // Use xor to detect changes from last stable state:
    // if a key has changed, it's bit will be 1, otherwise 0
    delta = sample ^ debounce_state_[row].debounced_state;

    // Increment counters and reset any unchanged bits:
    // increment bit 1 for all changed keys
    debounce_state_[row].db1 = ((debounce_state_[row].db1) ^ (debounce_state_[row].db0)) & delta;
    // increment bit 0 for all changed keys
    debounce_state_[row].db0 = ~(debounce_state_[row].db0) & delta;

    // Calculate returned change set: if delta is still true
    // and the counter has wrapped back to 0, the key is changed.

    changes = ~(~delta | (debounce_state_[row].db0) | (debounce_state_[row].db1));
    // Update state: in this case use xor to flip any bit that is true in changes.
    debounce_state_[row].debounced_state ^= changes;

    return changes;
  }

  RowState getRowState(uint8_t row) {
    return debounce_state_[row].debounced_state;
  }
};

}
}
}

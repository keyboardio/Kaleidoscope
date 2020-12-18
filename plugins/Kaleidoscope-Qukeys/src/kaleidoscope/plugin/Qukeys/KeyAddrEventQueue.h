// -*- mode: c++ -*-
/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2019  Keyboard.io, Inc.
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
//#include <assert.h>

#include "kaleidoscope/Runtime.h"
#include "kaleidoscope/KeyAddr.h"
#include "kaleidoscope/keyswitch_state.h"

namespace kaleidoscope {
namespace plugin {

// This class defines a keyswitch event queue that stores both press and release
// events, recording the key address, a timestamp, and the keyswitch state
// (press or release). It is optimized for random access to the queue entries,
// so that each property of each entry can be retrieved without fetching any
// other data, in order to best serve the specific needs of the Qukeys
// plugin. Its performance is better for a queue that needs to be searched much
// more frequently than entries are added or removed.
template <uint8_t _capacity,
          typename _Bitfield  = uint8_t,
          typename _Timestamp = uint16_t>
class KeyAddrEventQueue {

  static_assert(_capacity <= (sizeof(_Bitfield) * 8),
                "EventQueue error: _Bitfield type too small for _capacity!");

 private:
  uint8_t    length_{0};
  KeyAddr    addrs_[_capacity];
  _Timestamp timestamps_[_capacity]; // NOLINT(runtime/arrays)
  _Bitfield  release_event_bits_;

 public:
  uint8_t length() const {
    return length_;
  }
  bool isEmpty() const {
    return (length_ == 0);
  }
  bool isFull() const {
    return (length_ == _capacity);
  }

  // Queue entry access methods. Note: the caller is responsible for bounds
  // checking, because it's expected that a for loop will be used when searching
  // the queue, which will terminate when `index >= queue.length()`.
  KeyAddr addr(uint8_t index) const {
    // assert(index < length_);
    return addrs_[index];
  }

  _Timestamp timestamp(uint8_t index) const {
    // assert(index < length_);
    return timestamps_[index];
  }

  bool isRelease(uint8_t index) const {
    // assert(index < length_);
    return bitRead(release_event_bits_, index);
  }
  bool isPress(uint8_t index) const {
    // assert(index < length_);
    return !isRelease(index);
  }

  // Append a new event on the end of the queue. Note: the caller is responsible
  // for bounds checking; we don't guard against it here.
  void append(KeyAddr k, uint8_t keyswitch_state) {
    // assert(length_ < _capacity);
    addrs_[length_]      = k;
    timestamps_[length_] = Runtime.millisAtCycleStart();
    bitWrite(release_event_bits_, length_, keyToggledOff(keyswitch_state));
    ++length_;
  }

  // Remove the first event from the head of the queue, shifting the
  // others. This function actually shifts the queue by copying element values,
  // rather than using a ring buffer because we expect it will be called much
  // less often than the queue is searched via a for loop.
  void shift() {
    // assert(length > 0);
    --length_;
    for (uint8_t i{0}; i < length_; ++i) {
      addrs_[i]      = addrs_[i + 1];
      timestamps_[i] = timestamps_[i + 1];
    }
    release_event_bits_ >>= 1;
  }

  // Empty the queue entirely.
  void clear() {
    length_             = 0;
    release_event_bits_ = 0;
  }
};

}  // namespace plugin
}  // namespace kaleidoscope

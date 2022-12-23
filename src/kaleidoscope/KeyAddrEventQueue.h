// -*- mode: c++ -*-
/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2020  Keyboard.io, Inc.
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

#include <Arduino.h>  // for bitRead, bitWrite
#include <stdint.h>   // for uint8_t, uint16_t
//#include <assert.h>

#include "kaleidoscope/KeyAddr.h"          // for KeyAddr
#include "kaleidoscope/KeyEvent.h"         // for KeyEvent, KeyEventId
#include "kaleidoscope/Runtime.h"          // for Runtime, Runtime_
#include "kaleidoscope/key_defs.h"         // for Key_Undefined
#include "kaleidoscope/keyswitch_state.h"  // for IS_PRESSED, WAS_PRESSED, keyToggledOff

namespace kaleidoscope {

// This class defines a keyswitch event queue that stores both press and release
// events, recording the key address, a timestamp, and the keyswitch state
// (press or release). It is optimized for random access to the queue entries,
// so that each property of each entry can be retrieved without fetching any
// other data, in order to best serve the specific needs of the Qukeys
// plugin. Its performance is better for a queue that needs to be searched much
// more frequently than entries are added or removed.
template<uint8_t _capacity,
         typename _Bitfield  = uint8_t,
         typename _Timestamp = uint16_t>
class KeyAddrEventQueue {

  static_assert(_capacity <= (sizeof(_Bitfield) * 8),
                "EventQueue error: _Bitfield type too small for _capacity!");

 private:
  uint8_t length_{0};
  KeyEventId event_ids_[_capacity];   // NOLINT(runtime/arrays)
  KeyAddr addrs_[_capacity];          // NOLINT(runtime/arrays)
  _Timestamp timestamps_[_capacity];  // NOLINT(runtime/arrays)
  _Bitfield release_event_bits_;

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
  KeyEventId id(uint8_t index) const {
    // assert(index < length_);
    return event_ids_[index];
  }

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
  void append(const KeyEvent &event) {
    // assert(length_ < _capacity);
    event_ids_[length_]  = event.id();
    addrs_[length_]      = event.addr;
    timestamps_[length_] = Runtime.millisAtCycleStart();
    bitWrite(release_event_bits_, length_, keyToggledOff(event.state));
    ++length_;
  }

  // Remove an event from the head of the queue, shifting the subsequent
  // ones. This function actually shifts the queue by copying element values,
  // rather than using a ring buffer because we expect it will be called much
  // less often than the queue is searched via a for loop.
  void remove(uint8_t n = 0) {
    if (n >= length_ || length_ == 0)
      return;
    // assert(length > n);
    --length_;
    for (uint8_t i{n}; i < length_; ++i) {
      event_ids_[i]  = event_ids_[i + 1];
      addrs_[i]      = addrs_[i + 1];
      timestamps_[i] = timestamps_[i + 1];
    }
    // mask = all ones for bits >= n, zeros otherwise
    _Bitfield mask = _Bitfield(~0) << n;
    // use the inverse mask to get just the low bits (that won't be shifted)
    _Bitfield low_bits = release_event_bits_ & ~mask;
    // shift the event bits
    release_event_bits_ >>= 1;
    // use the mask to zero the low bits, leaving only the shifted high bits
    release_event_bits_ &= mask;
    // add the low bits back in
    release_event_bits_ |= low_bits;
  }

  void shift() {
    remove(0);
  }

  void shift(uint8_t n) {
    if (n >= length_) {
      clear();
      return;
    }
    length_ -= n;
    for (uint8_t i{0}; i < length_; ++i) {
      addrs_[i]      = addrs_[i + n];
      timestamps_[i] = timestamps_[i + n];
    }
    release_event_bits_ >>= n;
  }

  // Empty the queue entirely.
  void clear() {
    length_             = 0;
    release_event_bits_ = 0;
  }

  KeyEvent event(uint8_t i) const {
    uint8_t state = isRelease(i) ? WAS_PRESSED : IS_PRESSED;
    return KeyEvent{addr(i), state, Key_Undefined, id(i)};
  }

  // Only call this after `KeyEventTracker::shouldIgnore()` returns `true`.
  bool shouldAbort(const KeyEvent &event) const {
    // If the queue is empty, don't abort.
    if (length_ == 0)
      return false;
    // The compiler doesn't let us preserve the type of our integers here, so we
    // need to convert the difference back to int8_t to avoid a bug when it
    // overflows and the new event id is negative, but the old id is positive.
    KeyEventId offset = event.id() - event_ids_[0];
    // If the offset is negative, the event being processed is older than the
    // first event in the queue.  This shouldn't happen because the caller
    // should first check `KeyEventTracker::shouldIgnore()`, and only call this
    // function if that function returns `false`.
    return offset >= 0;
  }
};

}  // namespace kaleidoscope

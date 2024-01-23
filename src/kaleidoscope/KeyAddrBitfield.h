/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2020  Keyboard.io, Inc.
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

#include <Arduino.h>  // for bitClear, bitRead, bitSet, bitWrite
#include <stdint.h>   // for uint8_t
#include <string.h>   // for memset

#include "kaleidoscope/KeyAddr.h"  // for KeyAddr

namespace kaleidoscope {

// Return the number of `UnitType` units required to store `n` bits. Both `UnitType` &
// `WidthType` should be integer types. `WidthType` is whatever type the parameter `n` is
// stored as, and can be deduced by the compiler, so it's not necessary to declare it
// when calling this function (e.g. `bitfieldSize<uint16_t>(n)`). The default `UnitType`
// is `byte` (i.e. `uint8_t`, which is almost always what we want, so most of the time we
// can also drop that template parameter (e.g. `bitfieldSize(n)`).
template<typename _UnitType = uint8_t, typename _WidthType>
constexpr _WidthType bitfieldSize(_WidthType n) {
  return ((n - 1) / (8 * sizeof(_UnitType))) + 1;
}

// ================================================================================
// Generic Bitfield class, useful for defining KeyAddrBitfield, and others.
class KeyAddrBitfield {

 public:
  static constexpr uint8_t size         = KeyAddr::upper_limit;
  static constexpr uint8_t block_size   = 8 * sizeof(uint8_t);
  static constexpr uint8_t total_blocks = bitfieldSize<uint8_t>(size);

  static constexpr uint8_t blockIndex(KeyAddr k) {
    return k.toInt() / block_size;
  }
  static constexpr uint8_t bitIndex(KeyAddr k) {
    return k.toInt() % block_size;
  }
  static KeyAddr index(uint8_t block_index, uint8_t bit_index) {
    uint8_t offset = (block_index * block_size) + bit_index;
    return KeyAddr(offset);
  }
  bool read(KeyAddr k) const {
    // assert(k.toInt() < size);
    return bitRead(data_[blockIndex(k)], bitIndex(k));
  }
  void set(KeyAddr k) {
    // assert(k.toInt() < size);
    bitSet(data_[blockIndex(k)], bitIndex(k));
  }
  void clear(KeyAddr k) {
    // assert(k.toInt() < size);
    bitClear(data_[blockIndex(k)], bitIndex(k));
  }
  void write(KeyAddr k, bool value) {
    // assert(k.toInt() < size);
    bitWrite(data_[blockIndex(k)], bitIndex(k), value);
  }
  void clear() {
    memset(data_, 0, sizeof(data_));
  }

  // This function returns the number of set bits in the bitfield up to and
  // including the bit at index `k`. Two important things to note: it doesn't
  // verify that the bit for index `k` is set (the caller must do so first,
  // using `read()`), and what is returned is 1-indexed, so the caller will need
  // to subtract 1 before using it as an array index (e.g. when doing a `Key`
  // lookup for a sparse keymap layer).
  uint8_t ordinal(KeyAddr k) const {
    // assert(k.toInt() < size);
    uint8_t block_index = blockIndex(k);
    uint8_t count{0};
    for (uint8_t b{0}; b < block_index; ++b) {
      count += __builtin_popcount(data_[b]);
    }
    uint8_t last_data_unit = data_[block_index];
    last_data_unit &= ~(0xFF << bitIndex(k));
    count += __builtin_popcount(last_data_unit);
    return count;
  }

  uint8_t &block(uint8_t block_index) {
    // assert(block_index < total_blocks);
    return data_[block_index];
  }

 private:
  uint8_t data_[total_blocks] = {};


  // ----------------------------------------------------------------------------
  // Iterator!
 public:
  class Iterator {
   public:
    Iterator(const KeyAddrBitfield &bitfield, uint8_t x)
      : bitfield_(bitfield), block_index_(x) {}

    bool operator!=(const Iterator &other) {
      // First, the test for the end condition (return false when all the blocks have been
      // tested):
      while (block_index_ < other.block_index_) {
        // Get the data for the block at `block_index_` from the bitfield, then shift it
        // by the number of bits we've already checked (`bit_index_`):
        block_ = bitfield_.data_[block_index_];
        block_ >>= bit_index_;

        // Now we iterate through that block until we either find a bit that is set, or we
        // find that there are no more bits set. If (as expected most of the time) no bits
        // are set, we do nothing:
        while (block_ != 0) {
          // If the low (remaining) bit is set, generate an `KeyAddr` object from the
          // bitfield coordinates and store it for the dereference operator to return:
          if (block_ & 1) {
            index_ = KeyAddrBitfield::index(block_index_, bit_index_);
            return true;
          }
          // The low bit wasn't set, so we shift the data block by one and track that
          // shift with the bit coordinate (`bit_index_`):
          block_ >>= 1;
          bit_index_ += 1;
        }

        // When we're done checking a block, move on to the next one:
        block_index_ += 1;
        bit_index_ = 0;
      }
      return false;
    }

    KeyAddr operator*() {
      // assert(index_ < size);
      return index_;
    }

    void operator++() {
      ++bit_index_;
    }

   private:
    const KeyAddrBitfield &bitfield_;
    uint8_t block_index_;   // index of the block
    uint8_t bit_index_{0};  // bit index in the block
    uint8_t block_;
    KeyAddr index_;

  };  // class Iterator {

  friend class Iterator;

  Iterator begin() const {
    return Iterator{*this, 0};
  }
  Iterator end() const {
    return Iterator{*this, total_blocks};
  }

} __attribute__((packed));  // class KeyAddrBitfield {

}  // namespace kaleidoscope


// ================================================================================
// How to use the iterator above:
#if 0
// To use the KeyAddrBitfield::Iterator, write a loop like the following:
KeyAddrBitfield bitfield;
for (KeyAddr k : bitfield) {
  // Here, you'll get a `KeyAddr` object for each bit that is set in `bitfield`.
}
#endif

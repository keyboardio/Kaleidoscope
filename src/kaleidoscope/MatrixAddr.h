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

// IWYU pragma: private, include "kaleidoscope/KeyAddr.h"

#pragma once

#include <stdint.h>  // for uint8_t, int8_t

namespace kaleidoscope {

template<uint8_t rows__, uint8_t cols__>
class MatrixAddr {
 private:
  uint8_t offset_;

 public:
  typedef MatrixAddr<rows__, cols__> ThisType;

  static constexpr uint8_t rows        = rows__;
  static constexpr uint8_t cols        = cols__;
  static constexpr uint8_t upper_limit = rows__ * cols__;

  static constexpr uint8_t invalid_state = 255;

  static_assert(rows * cols < 255,
                "The number of rows and columns provided to instantiate \n"
                "MatrixAddr<rows, cols> exceeds the supported total number \n"
                "of 255 keys");

  constexpr MatrixAddr()
    : offset_(invalid_state) {}

  constexpr MatrixAddr(uint8_t row, uint8_t col)
    : offset_(row * cols + col) {}

  explicit constexpr MatrixAddr(uint8_t offset)
    : offset_(offset) {}

  // Rely on the default copy and move constructor.
  //
  // Note: If these were implemented naively as the commented versions below,
  //       some versions of avr-gcc (e.g. 4.9.2 or 5.4 would generate
  //       ridiculously bad assembler code for each copy construction,
  //       that would bloat the default firmware by 1K of PROGMEM!
  //
  constexpr MatrixAddr(const ThisType &other) = default;  // NOLINT(runtime/explicit)
  constexpr MatrixAddr(ThisType &&other)      = default;  // NOLINT(runtime/explicit)
  //constexpr MatrixAddr(const ThisType &other) : offset_(other.offset_) {}
  //constexpr MatrixAddr(ThisType &&other) : offset_(other.offset_) {}

  ThisType &operator=(const ThisType &) = default;
  ThisType &operator=(ThisType &&)      = default;

  template<typename MatrixAddr__>
  explicit constexpr MatrixAddr(const MatrixAddr__ &other)
    : MatrixAddr(other.row(), other.col()) {
    static_assert(MatrixAddr__::rows <= ThisType::rows,
                  "Matrix type conversion failed. Source type must not have greater row size than target type");
    static_assert(MatrixAddr__::cols <= ThisType::cols,
                  "Matrix type conversion failed. Source type must not have greater col size than target type");
  }

  // Get an invalid key address. Useful when initializing a state variable or
  // function parameter that handles invalid addresses.
  static constexpr ThisType none() {
    return ThisType(invalid_state);
  }

  // Invalidate the key address. Useful in plugins that need a state variable
  // that is sometimes valid, and sometimes not.
  void clear() {
    offset_ = invalid_state;
  }

  constexpr uint8_t row() const {
    return offset_ / cols;
  }
  constexpr uint8_t col() const {
    return offset_ % cols;
  }

  void setRow(uint8_t r) {
    //assert(r < rows);
    offset_ = this->col() + r * cols;
  }
  void setCol(uint8_t c) {
    //assert(c < cols);
    offset_ = this->row() * cols + c;
  }

  void shift(int8_t rows, int8_t cols) {
    offset_ += rows * cols + cols;
  }

  void rowShift(int8_t rows) {
    offset_ += rows * cols;
  }

  void colShift(int8_t cols) {
    offset_ += cols;
  }

  constexpr ThisType shifted(int8_t row_offset, int8_t col_offset) const {
    return ThisType(uint8_t(offset_ + row_offset * cols + col_offset));
  }

  constexpr ThisType rowShifted(int8_t row_offset) const {
    return ThisType(uint8_t(offset_ + row_offset * cols));
  }

  constexpr ThisType colShifted(int8_t col_offset) const {
    return ThisType(uint8_t(offset_ + col_offset));
  }

  constexpr uint8_t toInt() const {
    return offset_;
  }

  constexpr bool isValid() const {
    return offset_ < upper_limit;
  }

  ThisType operator++() {
    ++offset_;
    return *this;
  }

  ThisType operator++(int) {  // postfix ++
    ThisType copy(*this);
    ++*this;  // call the prefix increment
    return copy;
  }

  ThisType operator--() {
    --offset_;
    return *this;
  }

  ThisType operator--(int) {  // postfix ++
    ThisType copy(*this);
    --*this;  // call the prefix increment
    return copy;
  }

  template<typename MatrixAddr__>
  ThisType &operator+(const MatrixAddr__ &other) {
    *this = ThisType(this->row() + other.row(),
                     this->col() + other.col());
    return *this;
  }

  template<typename MatrixAddr__>
  ThisType &operator-(const MatrixAddr__ &other) {
    *this = ThisType(this->row() - other.row(),
                     this->col() - other.col());
    return *this;
  }

  template<typename MatrixAddr__>
  ThisType &operator+=(const MatrixAddr__ &other) {
    *this = *this + other;
    return *this;
  }

  template<typename MatrixAddr__>
  ThisType &operator-=(const MatrixAddr__ &other) {
    *this = *this - other;
    return *this;
  }

  bool operator==(const ThisType &other) const {
    return offset_ == other.offset_;
  }

  bool operator!=(const ThisType &other) const {
    return offset_ != other.offset_;
  }

  // Range is a helper class that is used in range based for loops
  // over all possible KeyAddr values.
  //
  struct Range {
    typedef ThisType Iterator;
    static constexpr ThisType begin() {
      return ThisType(uint8_t(0));
    }
    static constexpr ThisType end() {
      return ThisType(ThisType::upper_limit);
    }
  };

  // Use this method in range based for loops as e.g.
  // for(auto key_addr: KeyAddr::all()) { ... }
  //
  static constexpr Range all() {
    return Range{};
  }

  // This operator is needed to let MatrixAddr serve as Iterator type
  // to iterate over a Range.
  //
  constexpr const ThisType &operator*() const {
    return *this;
  }
};

// Comparison operators only used for module testing. This is because they
// cause ambiguous symbol lookup when used in the regular firmware.
//
// To use them also for the regular firmware they would need to be
// disambiguated by moving them to the Runtime.h header and replacing
// them with non-template versions that operate on the actual typedefed
// KeyAddr and KeyAddr.

#ifdef MATRIX_ADDR_TESTING

template<typename MatrixAddr1__, typename MatrixAddr2__>
bool operator==(const MatrixAddr1__ &a1, const MatrixAddr2__ &a2) {
  return (a1.row() == a2.row()) && (a1.col() == a2.col());
}
template<typename MatrixAddr1__, typename MatrixAddr2__>
bool operator!=(const MatrixAddr1__ &a1, const MatrixAddr2__ &a2) {
  return !operator==(a1, a2);
}

template<typename MatrixAddr1__, typename MatrixAddr2__>
bool operator>(const MatrixAddr1__ &a1, const MatrixAddr2__ &a2) {
  return (a1.row() > a2.row()) || ((a1.row() == a2.row()) && (a1.col() > a2.col()));
}

template<typename MatrixAddr1__, typename MatrixAddr2__>
bool operator<(const MatrixAddr1__ &a1, const MatrixAddr2__ &a2) {
  // This could be optimized if necessary
  return !operator>(a1, a2) && !operator==(a1, a2);
}

template<typename MatrixAddr1__, typename MatrixAddr2__>
bool operator>=(const MatrixAddr1__ &a1, const MatrixAddr2__ &a2) {
  // This could be optimized if necessary
  return operator>(a1, a2) || operator==(a1, a2);
}

template<typename MatrixAddr1__, typename MatrixAddr2__>
bool operator<=(const MatrixAddr1__ &a1, const MatrixAddr2__ &a2) {
  return !operator>(a1, a2);
}

#endif

}  // namespace kaleidoscope

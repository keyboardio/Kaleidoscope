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

#include <limits.h>
#include <stdint.h>

namespace kaleidoscope {

template<uint8_t rows__, uint8_t cols__>
class MatrixAddr {
 private:

  uint8_t offset_;

 public:

  typedef MatrixAddr<rows__, cols__> ThisType;

  static constexpr uint8_t rows = rows__;
  static constexpr uint8_t cols = cols__;
  static constexpr uint8_t matrix_size = rows__ * cols__;

  static constexpr uint8_t invalid_state = 255;

  static_assert(rows * cols < 255,
                "The number of rows and columns provided to instanciate \n"
                "MatrixAddr<rows, cols> exceeds the supported total number of 255 keys");

  constexpr MatrixAddr() : offset_(invalid_state) {}

  explicit constexpr MatrixAddr(uint8_t row, uint8_t col)
    : offset_(row * cols + col) {}

  explicit constexpr MatrixAddr(uint8_t offset)
    : offset_(offset) {}

  constexpr MatrixAddr(const ThisType &other) : offset_(other.offset_) {}

  template<typename MatrixAddr__>
  constexpr MatrixAddr(const MatrixAddr__ &other)
    : MatrixAddr(other.row(), other.col()) {}

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

  constexpr uint8_t offset() {
    return offset_;
  }

  constexpr operator uint8_t() {
    return offset_;
  }

  constexpr bool isValid() const {
    return (row() < rows__) && (col() < cols__);
  }

  ThisType &operator=(const ThisType &other) {
    offset_ = other.offset_;
    return *this;
  }

  template<typename MatrixAddr__>
  ThisType& operator=(const MatrixAddr__ &other) {
    offset_ = other.row() * cols + other.col();
    return *this;
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
};

// Comparison operators only used for module testing. This is because they
// cause ambiguous symbol lookup when used in the regular firmware.
//
// To use them also for the regular firmware they would need to be
// disambiguated by moving them to the Kaleidoscope.h header and replacing
// them with non-template versions that operate on the actual typedefed
// KeyAddr and LEDAddr.

#ifdef MATRIX_ADDR_TESTING

template<typename MatrixAddr1__, typename MatrixAddr2__>
bool operator==(const MatrixAddr1__&a1, const MatrixAddr2__ &a2) {
  return (a1.row() == a2.row()) && (a1.col() == a2.col());
}
template<typename MatrixAddr1__, typename MatrixAddr2__>
bool operator!=(const MatrixAddr1__&a1, const MatrixAddr2__ &a2) {
  return !operator==(a1, a2);
}

template<typename MatrixAddr1__, typename MatrixAddr2__>
bool operator>(const MatrixAddr1__&a1, const MatrixAddr2__ &a2) {
  return (a1.row() > a2.row())
         || ((a1.row() == a2.row()) && (a1.col() > a2.col()));
}

template<typename MatrixAddr1__, typename MatrixAddr2__>
bool operator<(const MatrixAddr1__&a1, const MatrixAddr2__ &a2) {
  // This could be optimized if necessary
  return !operator>(a1, a2) && !operator==(a1, a2);
}

template<typename MatrixAddr1__, typename MatrixAddr2__>
bool operator>=(const MatrixAddr1__&a1, const MatrixAddr2__ &a2) {
  // This could be optimized if necessary
  return operator>(a1, a2) || operator==(a1, a2);
}

template<typename MatrixAddr1__, typename MatrixAddr2__>
bool operator<=(const MatrixAddr1__&a1, const MatrixAddr2__ &a2) {
  return !operator>(a1, a2);
}

#endif

} // end namespace kaleidoscope

// Row/col based access functions have been superseded by matrix address
// base access. To deprecate any row/col based access functions, toggle
// the definitions of macro KS_ROW_COL_FUNC below
// and uncomment _DEPRECATED_MESSAGE_ROW_COL_FUNC.
//
#define KS_ROW_COL_FUNC

#if 0
#define _DEPRECATED_MESSAGE_ROW_COL_FUNC \
   "Row/col based access functions have been deprecated. Please use " \
   "the KeyAddr/LEDAddr based versions instead."
#define KS_ROW_COL_FUNC DEPRECATED(ROW_COL_FUNC)
#endif

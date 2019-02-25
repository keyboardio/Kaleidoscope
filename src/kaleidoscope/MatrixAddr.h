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

// This header contains two classes that aid
// indexed access to key and LED matrix entries.
// There are two different implementations that feature the same
// interface and are therefore interchangeable and also cooperate, one is
// more memory efficient, the other prefers speed when it comes to extracting
// row and column indices.
//
// You should always compare the performance of both on your hardware
// and then select the one that is better suited for your needs.
//
// As both implementations are compatible, it is possible to use one type of
// matrix address class for key addressing and another for LED addressing.
//
// To define the types of matrix address storage class that is used
// throughout the firmware for key and LED accessing, provide the following
// two typedefs in your keyboard hardware class.
//
// typedef MatrixAddr<4, 16> KeyAddr;
// typedef MatrixAddrCompressed<4, 16> LEDAddr;
//
// Those two types are then automatically used throughout the whole
// firmware for any type of addressing.

#pragma once

#include <limits.h>
#include <stdint.h>

namespace kaleidoscope {
namespace internal {
namespace matrix_addr {

constexpr unsigned log2(unsigned n, unsigned p = 0) {
  return (n <= 1) ? p : log2(n / 2, p + 1);
}

template<typename Int1__, typename Int2___>
constexpr Int1__ shiftNOr(Int1__ v, Int2___ s) {
  return v | (v >> s);
}

// The following functions are used to determine the optimal
// storage row size for compressed storage. They compute the next greater
// power of two for an unsigned integer value of a given size.

constexpr
uint32_t upperPowerOfTwo(uint32_t v) {
  // From https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2

  // The following is not possible in a c++11 constexpr function
//     v--;
//     v |= v >> 1;
//     v |= v >> 2;
//     v |= v >> 4;
//     v |= v >> 8;
//     v |= v >> 16;
//     v++;
//     return v;

  // Thus, we have to reformulate it a little...
  return shiftNOr(shiftNOr(shiftNOr(shiftNOr(shiftNOr(v - 1, 1), 2), 4), 8), 16) + 1;
}

constexpr
uint16_t upperPowerOfTwo(uint16_t v) {
  // From https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2

  // The following is not possible in a c++11 constexpr function
//     v--;
//     v |= v >> 1;
//     v |= v >> 2;
//     v |= v >> 4;
//     v |= v >> 8;
//     v++;
//     return v;

  // Thus, we have to reformulate it a little...
  return shiftNOr(shiftNOr(shiftNOr(shiftNOr(v - 1, 1), 2), 4), 8) + 1;
}

constexpr
uint8_t upperPowerOfTwo(uint8_t v) {
  // From https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2

  // The following is not possible in a c++11 constexpr function
//     v--;
//     v |= v >> 1;
//     v |= v >> 2;
//     v |= v >> 4;
//     v++;
//     return v;

  // Thus, we have to reformulate it a little...
  return shiftNOr(shiftNOr(shiftNOr(v - 1, 1), 2), 4) + 1;
}

template<typename Int__> struct IntLimitsTrait {};
template<> struct IntLimitsTrait<uint8_t> {
  static constexpr uint8_t max = 0xFF;
};
template<> struct IntLimitsTrait<uint16_t> {
  static constexpr uint16_t max = 0xFFFF;
};
template<> struct IntLimitsTrait<uint32_t> {
  static constexpr uint32_t max = 0xFFFFFFFF;
};

} // end namespace internal
} // end namespace matrix_addr

// The MatrixAddr template class is meant to be used when speed is
// more important than saving memory. It uses two bytes to store row and column
// indices. For most keyboardsw that feature less than 256 indices keys/LEDs/...
// the row/col information can be stored in a more memory efficient fashion.
// In that case if saving memory is paramount, use
// prefer template class MatrixAddrCompressed that is defined later on
// in this file.
//
template<uint8_t rows__, uint8_t cols__>
class MatrixAddr {
 private:
  uint8_t row_;
  uint8_t col_;

 public:

  static constexpr uint8_t rows = rows__;
  static constexpr uint8_t cols = cols__;

  static constexpr uint8_t row_size = cols__;

  static constexpr uint8_t matrix_size = rows__ * cols__;

  static constexpr uint8_t invalid_index = 255;

  constexpr MatrixAddr() : row_(invalid_index), col_(invalid_index) {}

  explicit constexpr MatrixAddr(uint8_t row, uint8_t col) : row_(row), col_(col) {}

  explicit constexpr MatrixAddr(uint8_t offset)
    : MatrixAddr(offset / cols, offset % cols) {}

  template<typename MatrixAddr__>
  constexpr MatrixAddr(const MatrixAddr__ &other) : MatrixAddr(other.row(), other.col()) {}

  constexpr uint8_t row() const {
    return row_;
  }
  constexpr uint8_t col() const {
    return col_;
  }

  void setRow(uint8_t r) {
    row_ = r;
  }
  void setCol(uint8_t c) {
    col_ = c;
  }

  constexpr uint8_t offset() {
    return row_ * cols + col_;
  }

  constexpr bool isValid() const {
    return (row_ < rows__) && (col_ < cols__);
  }

  template<typename MatrixAddr__>
  MatrixAddr& operator=(const MatrixAddr__ &other) {
    this->row_ = other.row();
    this->col_ = other.col();
    return *this;
  }

  template<typename MatrixAddr__>
  MatrixAddr &operator+(const MatrixAddr__ &other) {
    this->row_ += other.row();
    this->col_ += other.col();
    return *this;
  }

  template<typename MatrixAddr__>
  MatrixAddr &operator-(const MatrixAddr__ &other) {
    this->row_ -= other.row();
    this->col_ -= other.col();
    return *this;
  }

  template<typename MatrixAddr__>
  MatrixAddr &operator+=(const MatrixAddr__ &other) {
    *this = *this + other;
    return *this;
  }

  template<typename MatrixAddr__>
  MatrixAddr &operator-=(const MatrixAddr__ &other) {
    *this = *this - other;
    return *this;
  }
};

// This is a copressed storage version of MatrixAddr.
// It uses a common integer to store row and column indices.
// To enable fast row/col access, bitshifts are used instead of
// integer divisions to compute row indices. This is only possible
// by padding rows with unused entries to gain a row size that
// is a power of two.
//
template<uint8_t rows__, uint8_t cols__, typename StorageType__ = uint8_t>
class MatrixAddrCompressed {
 private:

  StorageType__ raw_;

 public:

  typedef MatrixAddrCompressed<rows__, cols__, StorageType__> ThisType;

  // Row size must be a power of two to enable fast index extraction.
  static constexpr uint8_t row_size
    = internal::matrix_addr::upperPowerOfTwo(cols__);

  static constexpr StorageType__ max_raw_value
    = internal::matrix_addr::IntLimitsTrait<StorageType__>::max;
  static constexpr StorageType__ invalid_state = max_raw_value;
  static constexpr uint8_t shift = internal::matrix_addr::log2(row_size);

  static constexpr uint8_t rows = rows__;
  static constexpr uint8_t cols = cols__;

  static constexpr uint8_t matrix_size = rows__ * cols__;

  static constexpr StorageType__ rowMask = StorageType__(max_raw_value << shift);
  static constexpr StorageType__ colMask = StorageType__(~rowMask);

  static_assert(rows * row_size < max_raw_value, "Please use a StorageType__ with a higher capacity");

  constexpr MatrixAddrCompressed() : raw_(invalid_state) {}

  explicit constexpr MatrixAddrCompressed(uint8_t row, uint8_t col) : raw_(row * row_size + col) {}

  explicit constexpr MatrixAddrCompressed(uint8_t offset)
    : MatrixAddrCompressed(offset / cols, offset % cols) {}

  constexpr MatrixAddrCompressed(const ThisType &other) : raw_(other.raw_) {}

  template<typename MatrixAddr__>
  constexpr MatrixAddrCompressed(const MatrixAddr__ &other)
    : MatrixAddrCompressed(other.row(), other.col()) {}

  constexpr uint8_t row() const {
    return raw_ >> shift;
  }
  constexpr uint8_t col() const {
    return raw_ & colMask;
  }

  void setRow(uint8_t r) {
    raw_ = (r << shift) | (raw_ & colMask);
  }
  void setCol(uint8_t c) {
    raw_ = (raw_ & rowMask) | c;
  }

  constexpr uint8_t offset() {
    return row() * cols + col();
  }

  constexpr bool isValid() const {
    return (row() < rows__) && (col() < cols__);
  }

  ThisType &operator=(const ThisType &other) {
    raw_ = other.raw_;
    return *this;
  }

  template<typename MatrixAddr__>
  ThisType& operator=(const MatrixAddr__ &other) {
    raw_ = other.row() * row_size + other.col();
    return *this;
  }

  template<typename MatrixAddr__>
  ThisType &operator+(const MatrixAddr__ &other) {
    *this = ThisType(this->row_ + other.row(),
                     this->col_ + other.col());
    return *this;
  }

  template<typename MatrixAddr__>
  ThisType &operator-(const MatrixAddr__ &other) {
    *this = ThisType(this->row_ - other.row(),
                     this->col_ - other.col());
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

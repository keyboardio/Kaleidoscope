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
   
namespace internal {
   
template<typename T1, typename T2>
struct IsSame {
   static constexpr bool value = false;
};

template<typename T>
struct IsSame<T, T> {
   static constexpr bool value = true;
};

template<typename T>
struct RemoveReference {
   typedef T type;
};

template<typename T>
struct RemoveReference<T&> {
   typedef T type;
};

template<bool B> struct Bool2Type {};

} // end namespace internal

template<uint8_t rows__, uint8_t cols__>
class MatrixAddr {
 private:

  uint8_t offset_;

 public:

  typedef MatrixAddr<rows__, cols__> ThisType;

  static constexpr uint8_t rows = rows__;
  static constexpr uint8_t cols = cols__;
  static constexpr uint8_t upper_limit = rows__ * cols__;

  static constexpr uint8_t invalid_state = 255;

  static_assert(rows * cols < 255,
                "The number of rows and columns provided to instanciate \n"
                "MatrixAddr<rows, cols> exceeds the supported total number \n"
                "of 255 keys");

  constexpr MatrixAddr() : offset_(invalid_state) {}

  constexpr MatrixAddr(uint8_t row, uint8_t col)
    : offset_(row * cols + col) {}

  constexpr MatrixAddr(uint8_t offset)
    : offset_(offset) {}

  constexpr MatrixAddr(const ThisType &other) : offset_(other.offset_) {}
  constexpr MatrixAddr(ThisType &&other) : offset_(other.offset_) {}

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

  constexpr uint8_t toInt() const {
    return offset_;
  }

//   constexpr operator uint8_t() {
//     return offset_;
//   }

  constexpr bool isValid() const {
    return offset_ < upper_limit;
  }

  ThisType &operator=(const ThisType &other) {
    offset_ = other.offset_;
    return *this;
  }

  ThisType operator++() {
    ++offset_;
    return *this;
  }

  ThisType operator++(int) { // postfix ++
    ThisType copy(*this);
    ++*this;         // call the prefix increment
    return copy;
  }

  ThisType operator--() {
    --offset_;
    return *this;
  }

  ThisType operator--(int) { // postfix ++
    ThisType copy(*this);
    --*this;         // call the prefix increment
    return copy;
  }
  
  // The following templates are necessary as we cannot have both,
  // a templated and a non templated version of operator=.
  // Because of this, we have to take a detour over auxiliary
  // assign methods, one that enables to assign the offset_ member directly,
  // in case a compatible MatrixAddr class is used and 
  // one that does a row/col conversion otherwise.
  
  template<uint8_t xrows__, uint8_t xcols__>
  static void assign(internal::Bool2Type<true>, ThisType &self, const MatrixAddr<xrows__, xcols__> &other) {
    self.offset_ = other.offset_;
  }
      
  template<uint8_t xrows__, uint8_t xcols__>
  static void assign(internal::Bool2Type<false>, ThisType &self, const MatrixAddr<xrows__, xcols__> &other) {
    self.offset_ = other.row() * cols + other.col();
  }
  
  // Normal assignment
  //
  template<typename MatrixAddr__>
  ThisType& operator=(const MatrixAddr__ & other) {
     assign(
        // A dummy value that is only passed to make the compiler select
        // the right overload of assign(...). It will be optimized away
        // by the compiler, i.e. not be put on the stack when
        // calling the function as the class IsSame is empty.
        //
        internal::Bool2Type<
           internal::IsSame<
              ThisType, 
              typename internal::RemoveReference<MatrixAddr__>::type
           >::value
        >(), 
        *this, other
     );
    return *this;
  }  
  
  // Move assignment
  //
  template<typename MatrixAddr__>
  ThisType& operator=(MatrixAddr__ && other) {
     assign(
        // A dummy value that is only passed to make the compiler select
        // the right overload of assign(...). It will be optimized away
        // by the compiler, i.e. not be put on the stack when
        // calling the function as the class IsSame is empty.
        //
        internal::Bool2Type<
           internal::IsSame<
              ThisType, 
              typename internal::RemoveReference<MatrixAddr__>::type
           >::value
        >(), 
        *this, other
     );
    return *this;
  }

  template<typename MatrixAddr__>
  ThisType &operator+(const MatrixAddr__ & other) {
    *this = ThisType(this->row() + other.row(),
                     this->col() + other.col());
    return *this;
  }

  template<typename MatrixAddr__>
  ThisType &operator-(const MatrixAddr__ & other) {
    *this = ThisType(this->row() - other.row(),
                     this->col() - other.col());
    return *this;
  }

  template<typename MatrixAddr__>
  ThisType &operator+=(const MatrixAddr__ & other) {
    *this = *this + other;
    return *this;
  }

  template<typename MatrixAddr__>
  ThisType &operator-=(const MatrixAddr__ & other) {
    *this = *this - other;
    return *this;
  }

  bool operator==(const ThisType &other) const {
    return offset_ == other.offset_;
  }  
  
  template<typename MatrixAddr__>
  bool operator==(const MatrixAddr__ & other) {
     return   (this->row() == other.row())
           && (this->col() == other.col());
  }

  class forward_iterator {

    ThisType addr_;

   public:

    constexpr explicit forward_iterator(ThisType addr) : addr_(addr) {}

    forward_iterator operator++() {
      ++addr_;
      return *this;
    }

    forward_iterator operator++(int) { // postfix ++
      forward_iterator copy(*this);
      ++*this;         // call the prefix increment
      return copy;
    }

    ThisType operator*() {
      return addr_;
    }

    bool operator==(const forward_iterator &other) {
      return addr_ == other.addr_;
    }
    bool operator!=(const forward_iterator &other) {
      return !this->operator==(other);
    }
  };

  typedef forward_iterator iterator;

  forward_iterator begin() const {
    return forward_iterator(ThisType(uint8_t(0)));
  }
  forward_iterator end() const {
    return forward_iterator(ThisType(upper_limit));
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
bool operator==(const MatrixAddr1__ & a1, const MatrixAddr2__ & a2) {
  return (a1.row() == a2.row()) && (a1.col() == a2.col());
}
template<typename MatrixAddr1__, typename MatrixAddr2__>
bool operator!=(const MatrixAddr1__ & a1, const MatrixAddr2__ & a2) {
  return !operator==(a1, a2);
}

template<typename MatrixAddr1__, typename MatrixAddr2__>
bool operator>(const MatrixAddr1__ & a1, const MatrixAddr2__ & a2) {
  return (a1.row() > a2.row())
         || ((a1.row() == a2.row()) && (a1.col() > a2.col()));
}

template<typename MatrixAddr1__, typename MatrixAddr2__>
bool operator<(const MatrixAddr1__ & a1, const MatrixAddr2__ & a2) {
  // This could be optimized if necessary
  return !operator>(a1, a2) && !operator==(a1, a2);
}

template<typename MatrixAddr1__, typename MatrixAddr2__>
bool operator>=(const MatrixAddr1__ & a1, const MatrixAddr2__ & a2) {
  // This could be optimized if necessary
  return operator>(a1, a2) || operator==(a1, a2);
}

template<typename MatrixAddr1__, typename MatrixAddr2__>
bool operator<=(const MatrixAddr1__ & a1, const MatrixAddr2__ & a2) {
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

/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2018  Keyboard.io, Inc.
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

// clang-format off

#pragma once

#include <Arduino.h>  // for PROGMEM
#include <stddef.h>   // for size_t
#include <stdint.h>   // for uint8_t

namespace kaleidoscope {
namespace bitfields {

namespace internal {

// To store the bitfield we use a recursively defined template struct.
// It contains one byte of storage and a nested template struct
// that wraps the remaining bytes. A specialization of the template
// is used to end the recursion.
//
// By tagging the struct with __attribute__((packed)), we make sure
// that no padding bytes are added between the struct instances
// if the struct is used on non-8 bit architectures. This is important
// to ensure that all bytes are stored contiguously like it is the
// case in an intrinsic array.
//
template<uint8_t Byte_Count__>
struct _Bitfield {

  uint8_t byte_;

  _Bitfield < Byte_Count__ - 1 > more_bytes_;

  // The constructor initializes the first eight bits that are stored
  // in this struct and passes the remaining bits on to the nested
  // more_bytes_ struct.
  //
  template<typename ... Bits__>
  constexpr _Bitfield(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3,
                      uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7,
                      Bits__ ... bits)
    :  byte_(b0 << 0
             |   b1 << 1
             |   b2 << 2
             |   b3 << 3
             |   b4 << 4
             |   b5 << 5
             |   b6 << 6
             |   b7 << 7),
       more_bytes_(bits...)
  {}

} __attribute__((packed));

// A specialization that stores any remaining 1 to 8 bits. Any bits that
// are unused are zeroed by default.
//
template<>
struct _Bitfield<1 /* the last byte */> {

  uint8_t byte_;

  constexpr _Bitfield(uint8_t b0 = 0, uint8_t b1 = 0, uint8_t b2 = 0, uint8_t b3 = 0,
                      uint8_t b4 = 0, uint8_t b5 = 0, uint8_t b6 = 0, uint8_t b7 = 0)
    :  byte_(b0 << 0
             |   b1 << 1
             |   b2 << 2
             |   b3 << 3
             |   b4 << 4
             |   b5 << 5
             |   b6 << 6
             |   b7 << 7)
  {}
} __attribute__((packed));

// A common base class that provides methods to access the bits in
// a _Bitfield instance by treating it as a raw array.
//
class _BaseBitfield {

 protected:

  static bool isBitSetP(const void *bit_field, uint8_t raw_pos);
  static void setBitP(void *bit_field, uint8_t raw_pos, uint8_t val);
  static bool isBitSetPROGMEM_P(const void *bit_field, uint8_t raw_pos);
};

} // end namespace internal

// This is the actual user class.
//
template<size_t BitCount__>
class Bitfield : public internal::_BaseBitfield {

 public:

  static constexpr size_t nBytesForBits(size_t n_bits) {
    return (n_bits % 8) ? n_bits / 8 + 1 : n_bits / 8;
  }

  static constexpr size_t n_bits_ = BitCount__;
  static constexpr size_t n_bytes_ = nBytesForBits(BitCount__);

  template<typename ... Bits__>
  explicit constexpr Bitfield(Bits__...bits) : bits_(bits...) {
    static_assert(sizeof...(Bits__) == n_bits_,
                  "Invalid number of bits supplied to Bitfield<BitCount__> constructor. \n"
                  "Compare the number of bits supplied with the provided template \n"
                  "parameter BitCount__ to find out what went wrong.\n");
  }

  bool isBitSet(uint8_t raw_pos) const {
    return isBitSetP(&bits_, raw_pos);
  }

  bool isBitSetPROGMEM(uint8_t raw_pos) const {
    return isBitSetPROGMEM_P(&bits_, raw_pos);
  }

  void setBit(uint8_t raw_pos, uint8_t val) {
    setBitP(&bits_, raw_pos, val);
  }

  // An operator to query bits.
  // Note: This assumes bitfield to be stored in PROGMEM. Use the
  //       access method isBitSet(raw_pos) otherwise.
  //
  constexpr bool operator[](uint8_t raw_pos) const {
    return this->isBitSetPROGMEM_P(&bits_, raw_pos);
  }

 private:

  internal::_Bitfield<n_bytes_> bits_;
};

// The method generateBitfield may be used to conveniently create bitfields.
//
// auto my_bitfield = generateBitfield(1, 2, 3);
//
// Note: Due to a restriction in all gcc versions < 6.0
//       this function cannot be called with the output of KEYMAP_STACKED(...)
//       or KEYMAP(...) as its arguments.
//
template<typename...Bits__>
constexpr Bitfield<sizeof...(Bits__)> generateBitfield(Bits__...bits) {
  return Bitfield<sizeof...(Bits__)>(bits...);
}

// This auxiliary method determines the number of entries in a list.
//
namespace internal {

template<typename...Args__>
constexpr size_t nListEntries(Args__...) {
  return sizeof...(Args__);
}

} // end namespace internal

// Defines a bitfield named VAR_NAME. Applies MODIFIER (could e.g. set to const PROGMEM).
//
#define BITFIELD__(VAR_NAME, MODIFIER, ...)                                    \
    constexpr size_t VAR_NAME##_n_bits                                  __NL__ \
         = kaleidoscope::bitfields::internal::nListEntries(__VA_ARGS__);__NL__ \
                                                                        __NL__ \
    MODIFIER kaleidoscope::bitfields::Bitfield<VAR_NAME##_n_bits>       __NL__ \
         VAR_NAME{__VA_ARGS__};

#define BITFIELD(VAR_NAME, ...) BITFIELD__(VAR_NAME,, __VA_ARGS__)
#define BITFIELD_PROGMEM(VAR_NAME, ...) BITFIELD__(VAR_NAME, const PROGMEM, __VA_ARGS__)

#ifdef PER_KEY_DATA
#define KEYMAP_BITFIELD(VAR_NAME, ...) \
    BITFIELD(VAR_NAME, PER_KEY_DATA(0 /*default for non-existent keys*/, __VA_ARGS__))

#define KEYMAP_BITFIELD_PROGMEM(VAR_NAME, ...) \
    BITFIELD_PROGMEM(VAR_NAME, PER_KEY_DATA(0 /*default for non-existent keys*/, __VA_ARGS__))
#endif

#ifdef PER_KEY_DATA_STACKED
#define KEYMAP_BITFIELD_STACKED(VAR_NAME, ...) \
    BITFIELD(VAR_NAME, PER_KEY_DATA_STACKED(0 /*default for non-existent keys*/, __VA_ARGS__))

#define KEYMAP_BITFIELD_STACKED_PROGMEM(VAR_NAME, ...) \
    BITFIELD_PROGMEM(VAR_NAME, PER_KEY_DATA_STACKED(0 /*default for non-existent keys*/, __VA_ARGS__))
#endif

} // end namespace bitfields
} // end namespace kaleidoscope

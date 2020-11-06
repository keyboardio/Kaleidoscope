/* -*- mode: c++ -*-
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

#include <cstdint>

namespace kaleidoscope {
namespace testing {

struct Millis {
  uint32_t value;

  constexpr bool operator==(const Millis& other) const {
    return this->value == other.value;
  }
  constexpr bool operator!=(const Millis& other) const {
    return !(*this == other);
  }
  constexpr bool operator<(const Millis& other) const {
    return this->value < other.value;
  }
  constexpr bool operator>(const Millis& other) const {
    return this->value > other.value;
  }
  constexpr bool operator<=(const Millis& other) const {
    return this->value <= other.value;
  }
  constexpr bool operator>=(const Millis& other) const {
    return this->value >= other.value;
  }

  Millis operator+(const Millis& other) {
    return Millis{this->value + other.value};
  }
  Millis operator-(const Millis& other) {
    return Millis{this->value - other.value};
  }
  Millis operator+(const uint32_t raw) {
    return Millis{this->value + raw};
  }
  Millis operator-(const uint32_t raw) {
    return Millis{this->value - raw};
  }

  Millis& operator=(const uint32_t raw) {
    this->value = raw;
    return *this;
  }
  Millis& operator+=(const uint32_t raw) {
    this->value += raw;
    return *this;
  }
  Millis& operator-=(const uint32_t raw) {
    this->value -= raw;
    return *this;
  }

  constexpr bool operator<(uint32_t raw) const {
    return this->value < raw;
  }
  constexpr bool operator>(uint32_t raw) const {
    return this->value > raw;
  }
  constexpr bool operator<=(uint32_t raw) const {
    return this->value <= raw;
  }
  constexpr bool operator>=(uint32_t raw) const {
    return this->value >= raw;
  }

  operator size_t() {
     return value;
  }
};

constexpr bool operator<(uint32_t x, const Millis& y) {
  return x < y.value;
}
constexpr bool operator>(uint32_t x, const Millis& y) {
  return x > y.value;
}
constexpr bool operator<=(uint32_t x, const Millis& y) {
  return x <= y.value;
}
constexpr bool operator>=(uint32_t x, const Millis& y) {
  return x >= y.value;
}

constexpr Millis operator+(uint32_t x, const Millis& y) {
  return Millis{x + y.value};
}
constexpr Millis operator-(uint32_t x, const Millis& y) {
  return Millis{x - y.value};
}

}  // namespace testing
}  // namespace kaleidoscope

/* -*- mode: c++ -*-
 * BaseLeds -- LED hardware driver base class for Kaleidoscope
 * Copyright (C) 2019  Keyboard.io, Inc
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


#ifndef CRGB
#error cRGB and CRGB *must* be defined before including this header!
#endif

namespace kaleidoscope {
namespace driver {

template <typename _LedsDescription>
class BaseLeds {
 public:
  BaseLeds() {}

  static constexpr typename _LedsDescription::LedCountType led_count = _LedsDescription::led_count;
  typedef typename _LedsDescription::LedCountType LedCountType;

  void setup() {}
  void syncLeds(void) {}
  void setCrgbAt(typename _LedsDescription::LedCountType i, cRGB color) {}
  cRGB getCrgbAt(typename _LedsDescription::LedCountType i) {
    cRGB c = {
      0, 0, 0
    };
    return c;
  }
  typename _LedsDescription::LedCountType getLedIndex(KeyAddr key_addr) {
    return -1;
  }
};

}
}

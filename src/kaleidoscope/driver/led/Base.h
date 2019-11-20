/* -*- mode: c++ -*-
 * kaleidoscope::driver::led::Base -- LED hardware driver base class for Kaleidoscope
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
namespace led {

struct BaseProps {
  static constexpr uint8_t led_count = 0;
};

template <typename _LEDDriverProps>
class Base {
 public:
  Base() {}

  void setup() {}
  void syncLeds(void) {}
  void setCrgbAt(uint8_t i, cRGB color) {}
  cRGB getCrgbAt(uint8_t i) {
    cRGB c = {
      0, 0, 0
    };
    return c;
  }
  uint8_t getLedIndex(uint8_t key_offset) {
    return 0;
  }

  static class LEDs {
   private:
    uint8_t offset_;
   public:
    LEDs() : offset_(0) {}
    LEDs(uint8_t offset) : offset_(offset) {}

    typedef LEDs ThisType;

    constexpr uint8_t offset() {
      return offset_;
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

    bool operator==(const ThisType &other) const {
      return offset_ == other.offset_;
    }

    bool operator!=(const ThisType &other) const {
      return offset_ != other.offset_;
    }

    struct Range {
      typedef ThisType Iterator;
      static constexpr ThisType begin() {
        return ThisType(uint8_t(0));
      }
      static constexpr ThisType end() {
        return ThisType(_LEDDriverProps::led_count);
      }
    };

    static constexpr Range all() {
      return Range{};
    }
    constexpr const ThisType &operator*() const {
      return *this;
    }
    constexpr bool isValid(uint8_t index) {
      return (_LEDDriverProps::led_count > 0 && index < _LEDDriverProps::led_count);
    }
  } LEDs_;

 protected:
  typedef _LEDDriverProps Props_;
};

}
}
}

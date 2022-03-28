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

#include <Arduino.h>  // for PROGMEM, pgm_read_byte
#include <stdint.h>   // for uint8_t

namespace kaleidoscope {
namespace driver {
namespace led {

constexpr uint8_t no_led = uint8_t(-1);

struct BaseProps {
  static constexpr uint8_t led_count = 0;

  // C++ does not allow empty constexpr arrays
  //
  static constexpr uint8_t key_led_map[] PROGMEM = {no_led};
};

template<typename _LEDDriverProps>
class Base {
 public:
  Base() {}

  void setup() {}
  void syncLeds(void) {}
  void setCrgbAt(uint8_t i, cRGB color) {}
  cRGB getCrgbAt(uint8_t i) {
    cRGB c = {
      0, 0, 0};
    return c;
  }
  void setBrightness(uint8_t brightness) {}
  uint8_t getBrightness() {
    return 255;
  }

  static uint8_t getLedIndex(uint8_t key_offset) {

    // Give the compiler the oportunity to optimize
    // for boards without LEDs.
    //
    if (_LEDDriverProps::led_count == 0) {
      return no_led;
    }

    if (key_offset >= sizeof(_LEDDriverProps::key_led_map)) {
      return no_led;
    }

    return pgm_read_byte(&_LEDDriverProps::key_led_map[key_offset]);
  }

  class LEDRangeIterator {
   private:
    uint8_t offset_;

   public:
    LEDRangeIterator()
      : offset_(0) {}
    explicit LEDRangeIterator(uint8_t offset)
      : offset_(offset) {}

    typedef LEDRangeIterator ThisType;

    constexpr uint8_t offset() const {
      return offset_;
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

    bool operator==(const ThisType &other) const {
      return offset_ == other.offset_;
    }

    bool operator!=(const ThisType &other) const {
      return offset_ != other.offset_;
    }

    struct Range {
      typedef LEDRangeIterator Iterator;
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
    constexpr bool isValid(uint8_t index) const {
      return (_LEDDriverProps::led_count > 0 && index < _LEDDriverProps::led_count);
    }
  };

  static LEDRangeIterator &LEDs() {
    static LEDRangeIterator leds;
    return leds;
  }

 protected:
  typedef _LEDDriverProps Props_;
};

}  // namespace led
}  // namespace driver
}  // namespace kaleidoscope

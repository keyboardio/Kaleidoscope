/* -*- mode: c++ -*-
 * kaleidoscope::util::flasher::Base -- Base flasher utility class
 * Copyright (C) 2019  Keyboard.io, Inc
 * Copyright (C) 2019  Dygma Lab S.L.
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

#include <stdint.h>  // for uint8_t

namespace kaleidoscope {
namespace util {
namespace flasher {

struct BaseProps {
  static constexpr uint8_t page_size  = 64;
  static constexpr uint8_t frame_size = 16;
  static constexpr uint8_t blank      = 0xff;
  static constexpr uint8_t delay      = 1;

  static struct command {
    static constexpr uint8_t page_address        = 0x01;
    static constexpr uint8_t continue_page       = 0x02;
    static constexpr uint8_t execute             = 0x03;
    static constexpr uint8_t erase_program       = 0x04;
    static constexpr uint8_t get_version_and_crc = 0x06;
  } command;
};

template<typename _Props>
class Base {
 public:
  Base() {}

  template<typename T>
  static uint8_t flash(uint8_t address, T &firmware) {
    return 0;
  }
  template<typename T>
  static uint8_t verify(uint8_t address, T &firmware) {
    return 0;
  }
  static uint8_t command(uint8_t address, uint8_t command) {
    return 0;
  }
};

}  // namespace flasher
}  // namespace util
}  // namespace kaleidoscope

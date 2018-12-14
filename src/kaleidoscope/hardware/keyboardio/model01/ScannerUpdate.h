/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Model01 -- Keyboard.io Model01 hardware support for Kaleidoscope
 * Copyright (C) 2017-2018  Keyboard.io, Inc
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

#ifdef ARDUINO_AVR_MODEL01

namespace kaleidoscope {
namespace hardware {
namespace keyboardio {
namespace model01 {
class ScannerUpdate {
 public:
  ScannerUpdate() {}

  static void update();
 private:
  static void reset_left_attiny();
  static void reset_right_attiny();
  static uint8_t read_crc16(byte addr, byte *version,
                            uint16_t *crc16, uint16_t offset,
                            uint16_t length);
  static void get_version(byte addr);
  static int erase_program(uint8_t addr);
  static int write_firmware(uint8_t addr);
  static int verify_firmware(byte addr);
  static byte update_attiny(byte addr);
};
}
}
}
}

#endif

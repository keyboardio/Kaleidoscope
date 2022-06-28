/* -*- mode: c++ -*-
 * Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2022  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#if defined(__AVR__)

#include <stdint.h>  // for uint8_t
#include "kaleidoscope/device/avr/pins_and_ports.h"

namespace kaleidoscope {
namespace driver {
namespace sensor {

struct ADNS5050BaseProps {
  static constexpr uint8_t clock_pin = NO_PIN;
  static constexpr uint8_t io_pin = NO_PIN;
  static constexpr uint8_t chip_select_pin = NO_PIN;
};

template <typename _Props>
class ADNS5050 {
 public:
  ADNS5050() {}

  typedef struct Report {
    int8_t dx;
    int8_t dy;
  };

  enum class CPI {
    CPI125 = 0x11,
    CPI250 = 0x12,
    CPI375 = 0x13,
    CPI500 = 0x14,
    CPI625 = 0x15,
    CPI750 = 0x16,
    CPI875 = 0x17,
    CPI1000 = 0x18,
    CPI1125 = 0x19,
    CPI1250 = 0x1a,
    CPI1375 = 0x1b
  };

  void setup() {
    static_assert(_Props::clock_pin != NO_PIN, "No clock pin specified!");
    static_assert(_Props::io_pin != NO_PIN, "No IO pin specified!");
    static_assert(_Props::chip_select_pin != NO_PIN, "No chip select pin specified!");

    DDR_OUTPUT(_Props::clock_pin);
    DDR_OUTPUT(_Props::io_pin);
    DDR_OUTPUT(_Props::chip_select_pin);

    // 0x5a is chip reset (datasheet page 26)
    writeRegister(Register::CHIP_RESET, 0x5a);
    delay(55);

    // read an initial report from the sensor, and discard it.
    readReport();
  }

  void sync() {
    cs_select();
    delayMicroseconds(1);
    cs_deselect();
  }

  Report readReport() {
    Report report;

    cs_select();

    serialWrite(Register::MOTION_BURST);

    uint8_t x = serialRead();
    uint8_t y = serialRead();

    cs_deselect();

    report.dx = convertTwosComp(x);
    report.dy = convertTwosComp(y);

    return report;
  }

  void setCPI(CPI cpi) {
    uint8_t cpival = cpi / 125;
    if (cpival < 0x01) {
      cpival = 0x1;
    }
    if (cpival > 0x0d) {
      cpival = 0x0d;
    }

    writeRegister(Register::MOUSE_CONTROL2, 0b10000 | cpival);
  }
  CPI getCPI() {
    // not implemented yet
    return CPI::CPI125;
  }

  bool checkSignature() {
    uint8_t rid = readRegister(Register::REVISION_ID);
    uint8_t pid = readRegister(Register::PRODUCT_ID);
    uint8_t pid2 = readRegister(Register::PRODUCT_ID2);

    return (pid == 0x12 && rid == 0x01 && pid2 == 0x26);
  }

 public:
  enum class Register {
    PRODUCT_ID = 0x00,
    REVISION_ID = 0x01,
    MOTION = 0x02,
    DELTA_X = 0x03,
    DELTA_Y = 0x04,
    MOUSE_CONTROL2 = 0x19,
    CHIP_RESET = 0x3a,
    PRODUCT_ID2 = 0x3e,
    MOTION_BURST = 0x63
  };

  int8_t convertTwosComp(uint8_t data) {
    if ((data & 0x80) == 0x80)
      return -128 + (data & 0x7F);
    else
      return data;
  }

  uint8_t serialRead() {
    DDR_INPUT(_Props::io_pin);
    delayMicroseconds(1);

    uint8_t byte;

    for (uint8_t i = 0; i < 8; i++) {
      OUTPUT_LOW(_Props::clock_pin);
      delayMicroseconds(1);

      byte = (byte  << 1) | READ_PIN(_Props::io_pin);

      OUTPUT_HIGH(_Props::clock_pin);
      delayMicroseconds(1);
    }

    return byte;
  }

  void serialWrite(uint8_t data) {
    DDR_OUTPUT(_Props::io_pin);
    delayMicroseconds(1);

    for (int8_t i = 7; i >= 0; i--) {
      OUTPUT_LOW(_Props::clock_pin);
      delayMicroseconds(1);

      if (data && (1 << i)) {
        OUTPUT_HIGH(_Props::io_pin);
      } else {
        OUTPUT_LOW(_Props::io_pin);
      }

      delayMicroseconds(2);

      OUTPUT_HIGH(_Props::clock_pin);
      delayMicroseconds(1);
    }

    delayMicroseconds(4);
  }
  void serialWrite(Register reg) {
    serialWrite((uint8_t) reg);
  }

  uint8_t readRegister(Register reg) {
    cs_select();

    serialWrite(reg);
    uint8_t byte = serialRead();

    delayMicroseconds(1);

    cs_deselect();

    return byte;
  }
  void writeRegister(Register reg, uint8_t data) {
    cs_select();
    serialWrite(0b10000000 | (uint8_t)reg);
    serialWrite(data);
    cs_deselect();
  }

  void cs_select() {
    OUTPUT_LOW(_Props::chip_select_pin);
    delayMicroseconds(1);
  }
  void cs_deselect() {
    OUTPUT_HIGH(_Props::chip_select_pin);
    delayMicroseconds(1);
  }

};

}  // namespace sensor
}  // namespace driver
}  // namespace kaleidoscope

#endif  // define(__AVR__)

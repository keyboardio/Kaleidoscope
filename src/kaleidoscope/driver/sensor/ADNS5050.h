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
  struct pin {
    static constexpr uint8_t SCLK = NO_PIN;
    static constexpr uint8_t SDIO = NO_PIN;
    static constexpr uint8_t NCS = NO_PIN;
  };
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
    static_assert(_Props::pin::SCLK != NO_PIN, "No clock pin specified!");
    static_assert(_Props::pin::SDIO != NO_PIN, "No IO pin specified!");
    static_assert(_Props::pin::NCS != NO_PIN, "No chip select pin specified!");

    DDR_OUTPUT(_Props::pin::SCLK);
    DDR_OUTPUT(_Props::pin::SDIO);
    DDR_OUTPUT(_Props::pin::NCS);

    // 0x5a is chip reset (datasheet page 26)
    writeRegister(Register::CHIP_RESET, 0x5a);
    delay(55);

    // read an initial report from the sensor, and discard it.
    // readReport();
  }

  void sync() {
    OUTPUT_LOW(_Props::pin::NCS);
    delayMicroseconds(1);
    OUTPUT_HIGH(_Props::pin::NCS);
  }

  Report readReport() {
    Report report;

    OUTPUT_LOW(_Props::pin::NCS);

    serialWrite(Register::MOTION_BURST);

    uint8_t x = serialRead();
    uint8_t y = serialRead();

    OUTPUT_HIGH(_Props::pin::NCS);

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
  struct Timeout {
    static constexpr uint16_t WAKEUP = 55000;
    static constexpr uint16_t SWW = 30;
    static constexpr uint16_t SWR = 20;
    static constexpr uint16_t SRW = 1;
    static constexpr uint16_t SRR = 1;
    static constexpr uint16_t SRAD = 4;
  };
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
    DDR_INPUT(_Props::pin::SDIO);
    delayMicroseconds(1);

    uint8_t byte;

    for (uint8_t i = 0; i < 8; i++) {
      OUTPUT_LOW(_Props::pin::SCLK);
      delayMicroseconds(1);

      byte = (byte  << 1) | READ_PIN(_Props::pin::SDIO);

      OUTPUT_HIGH(_Props::pin::SCLK);
      delayMicroseconds(1);
    }

    return byte;
  }

  void serialWrite(uint8_t data) {
    DDR_OUTPUT(_Props::pin::SDIO);
    delayMicroseconds(1);

    for (int8_t i = 7; i >= 0; i--) {
      OUTPUT_LOW(_Props::pin::SCLK);
      delayMicroseconds(1);

      if (data && (1 << i)) {
        OUTPUT_HIGH(_Props::pin::SDIO);
      } else {
        OUTPUT_LOW(_Props::pin::SDIO);
      }

      delayMicroseconds(2);

      OUTPUT_HIGH(_Props::pin::SCLK);
      delayMicroseconds(1);
    }

    delayMicroseconds(4);
  }
  void serialWrite(Register reg) {
    serialWrite((uint8_t) reg);
  }

  uint8_t readRegister(Register reg) {
    OUTPUT_LOW(_Props::pin::NCS);

    serialWrite(reg);
    uint8_t byte = serialRead();

    delayMicroseconds(1);

    OUTPUT_HIGH(_Props::pin::NCS);

    return byte;
  }
  void writeRegister(Register reg, uint8_t data) {
    OUTPUT_LOW(_Props::pin::NCS);
    serialWrite(0b10000000 | (uint8_t)reg);
    serialWrite(data);
    OUTPUT_HIGH(_Props::pin::NCS);
  }
};

}  // namespace sensor
}  // namespace driver
}  // namespace kaleidoscope

#endif  // define(__AVR__)

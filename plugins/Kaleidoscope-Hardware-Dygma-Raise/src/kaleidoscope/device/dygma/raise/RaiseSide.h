/* -*- mode: c++ -*-
 * kaleidoscope::device::dygma::Raise -- Kaleidoscope device plugin for Dygma Raise
 * Copyright (C) 2017-2020  Keyboard.io, Inc
 * Copyright (C) 2017-2019  Dygma Lab S.L.
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

#ifdef ARDUINO_SAMD_RAISE

#include <Arduino.h>

#include "TWI.h"

struct cRGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

namespace kaleidoscope {
namespace device {
namespace dygma {
namespace raise {

#define LED_BANKS          9

#define LEDS_PER_HAND      72
#define LPH                LEDS_PER_HAND
#define LEDS_PER_BANK      8
#define LED_BYTES_PER_BANK (sizeof(cRGB) * LEDS_PER_BANK)

typedef union {
  cRGB leds[LEDS_PER_HAND];
  uint8_t bytes[LED_BANKS][LED_BYTES_PER_BANK];
} LEDData_t;

// return what bank the led is in
#define LED_TO_BANK(led) (led / LEDS_PER_BANK)

typedef union {
  uint8_t rows[5];
  uint64_t all;
} keydata_t;

class RaiseSide {
 public:
  explicit RaiseSide(uint8_t ad01)
    : ad01_(ad01), twi_(i2c_addr_base_ | ad01) {}

  int readVersion();
  int readSLEDVersion();
  int readSLEDCurrent();
  uint8_t setSLEDCurrent(uint8_t current);
  int readJoint();
  int readLayout();

  uint8_t setKeyscanInterval(uint8_t delay);
  int readKeyscanInterval();

  uint8_t setLEDSPIFrequency(uint8_t frequency);
  int readLEDSPIFrequency();

  bool moreKeysWaiting();
  void sendLEDData();
  void sendLEDBank(uint8_t bank);
  keydata_t getKeyData();
  bool readKeys();
  uint8_t controllerAddress();
  uint8_t crc_errors() {
    return twi_.crc_errors();
  }

  void setBrightness(uint8_t brightness) {
    brightness_adjustment_ = 255 - brightness;
  }
  uint8_t getBrightness() {
    return 255 - brightness_adjustment_;
  }

  LEDData_t led_data;
  bool online = false;

 private:
  uint8_t brightness_adjustment_ = 0;
  int ad01_;
  TWI twi_;
  keydata_t key_data_;
  uint8_t next_led_bank_ = 0;

  static constexpr uint8_t i2c_addr_base_ = 0x58;

  int readRegister(uint8_t cmd);
};

}  // namespace raise
}  // namespace dygma
}  // namespace device
}  // namespace kaleidoscope

#endif

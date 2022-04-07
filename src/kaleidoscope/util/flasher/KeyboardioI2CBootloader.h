/* -*- mode: c++ -*-
 * kaleidoscope::util::flasher::KeyboardioI2CBootloader -- Flasher for Keyboardio's I2C Bootloader
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

// TODO(@algernon): We should support AVR here, too.
#ifdef __SAMD21G18A__

#include <Wire.h>
#include <stdint.h>

#include "kaleidoscope/util/crc16.h"
#include "kaleidoscope/util/flasher/Base.h"

namespace kaleidoscope {
namespace util {
namespace flasher {

template<typename _Props>
class KeyboardioI2CBootloader : kaleidoscope::util::flasher::Base<_Props> {
 public:
  template<typename T>
  static bool flash(uint8_t address, T &firmware) {
    if (!verify(address, firmware)) {
      return false;
    }

    if (!erase_program(address)) {
      return false;
    }
    if (!write_firmware(address, firmware)) {
      return false;
    }
    if (!verify_firmware(address, firmware)) {
      return false;
    }

    return command(address, _Props::command.execute) == 0 ? true : false;
  }

  template<typename T>
  static bool verify(uint8_t address, T &firmware) {
    CRCAndVersion crc_and_version = get_version(address, firmware);
    return (crc_and_version.version != 0xff) && (crc_and_version.crc != 0xffff);
  }

  static uint8_t command(uint8_t address, uint8_t command) {
    Wire.beginTransmission(address);
    Wire.write(command);
    Wire.write(0x00);
    uint8_t result = Wire.endTransmission();
    return result;
  }

 private:
  struct CRCAndVersion {
    uint8_t version;
    uint16_t crc;
  };

  static uint8_t read_crc16(uint8_t addr,
                            CRCAndVersion *crc_and_version,
                            uint16_t offset,
                            uint16_t length) {
    uint8_t result;

    Wire.beginTransmission(addr);
    Wire.write(_Props::command.get_version_and_crc);
    Wire.write(offset & 0xff);  // addr (lo)
    Wire.write(offset >> 8);    // addr (hi)
    Wire.write(length & 0xff);  // len (lo)
    Wire.write(length >> 8);    // len (hi)
    result = Wire.endTransmission(false);
    if (result != 0) {
      return result;
    }

    // wait for cksum to be calculated - takes about 20ms
    delay(100);

    Wire.requestFrom(addr, 3);
    uint8_t v                = Wire.read();
    crc_and_version->version = v;
    if (Wire.available() == 0) {
      return 0xFF;
    }
    uint8_t crc16_lo = Wire.read();
    if (Wire.available() == 0) {
      return 0xFF;
    }
    uint8_t crc16_hi = Wire.read();
    while (Wire.available()) {
      uint8_t c = Wire.read();
    }
    crc_and_version->crc = (crc16_hi << 8) | crc16_lo;
    return result;
  }

  template<typename T>
  static CRCAndVersion get_version(uint8_t addr, T &firmware) {
    static CRCAndVersion crc_and_version = {0xff, 0xff};

    // This here to resolve some weird I2C startup bug.
    // Usually in the RHS, get_version fails with the I2C master writing the
    // address and the CRC request (0x06), the CRC parameters are never written
    // doing a read first seems to let things settle in a way that allows the
    // right to respond correctly
    Wire.requestFrom(addr, (uint8_t)3);
    while (Wire.available()) {
      // throw away the info, as cksum calculation request has yet to be issued.
      Wire.read();
    }

    int result = read_crc16(addr, &crc_and_version, firmware.offsets[0] + 4, firmware.length - 4);
    return crc_and_version;
  }

  static bool erase_program(uint8_t addr) {
    Wire.beginTransmission(addr);
    Wire.write(_Props::command.erase_program);
    uint8_t result = Wire.endTransmission();

    // wait for erase
    delay(1000);

    return result != 0;
  }

  template<typename T>
  static bool write_firmware(uint8_t addr, T &firmware) {
    uint8_t result;
    uint8_t o = 0;

    for (uint16_t i = 0; i < firmware.length; i += _Props::page_size) {
      Wire.beginTransmission(addr);
      Wire.write(_Props::command.page_address);
      Wire.write(firmware.offsets[o] & 0xff);
      Wire.write(firmware.offsets[o] >> 8);
      result = Wire.endTransmission();
      delay(_Props::delay);

      // got something other than ACK. Start over.
      if (result != 0) {
        return false;
      }

      // transmit each frame separately
      for (uint8_t frame = 0; frame < _Props::page_size / _Props::frame_size; frame++) {
        Wire.beginTransmission(addr);
        Wire.write(_Props::command.continue_page);
        uint16_t crc16 = 0xffff;
        for (uint8_t j = frame * _Props::frame_size;
             j < (frame + 1) * _Props::frame_size;
             j++) {
          if (i + j < firmware.length) {
            uint8_t b = pgm_read_byte(&firmware.data[i + j]);
            Wire.write(b);
            crc16 = _crc16_update(crc16, b);
          } else {
            Wire.write(_Props::blank);
            crc16 = _crc16_update(crc16, _Props::blank);
          }
        }
        // write the CRC16, little end first
        Wire.write(crc16 & 0xff);
        Wire.write(crc16 >> 8);
        Wire.write(0x00);  // dummy end uint8_t
        result = Wire.endTransmission();
        // got something other than NACK. Start over.
        if (result != 3) {
          return false;
        }
        delay(_Props::delay);
      }
      o++;
    }
    return true;
  }

  template<typename T>
  static bool verify_firmware(uint8_t addr, T &firmware) {
    uint8_t result = 3;
    CRCAndVersion crc_and_version;

    while (result != 0) {
      // skip the first 4 uint8_ts, are they were probably overwritten by the
      // reset vector preservation
      result = read_crc16(addr, &crc_and_version, firmware.offsets[0] + 4, firmware.length - 4);
      if (result != 0) {
        delay(100);
        continue;
      }
    }

    // calculate our own CRC16
    uint16_t check_crc16 = 0xffff;
    for (uint16_t i = 4; i < firmware.length; i++) {
      check_crc16 = _crc16_update(check_crc16, pgm_read_byte(&(firmware.data[i])));
    }

    return crc_and_version.crc == check_crc16;
  }
};

}  // namespace flasher
}  // namespace util
}  // namespace kaleidoscope

#endif

/* -*- mode: c++ -*-
 * kaleidoscope::device::dygma::Raise -- Kaleidoscope device plugin for Dygma Raise
 * Copyright (C) 2017-2019  Keyboard.io, Inc
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

#ifdef ARDUINO_SAMD_RAISE

#include <Wire.h>

#include "TWI.h"
#include "kaleidoscope/util/crc16.h"

namespace kaleidoscope {
namespace device {
namespace dygma {
namespace raise {

uint8_t TWI::writeTo(uint8_t *data, size_t length) {
  Wire.beginTransmission(addr_);

  // calc cksum
  uint16_t crc16  = 0xffff;
  uint8_t *buffer = data;
  for (uint8_t i = 0; i < length; i++) {
    crc16 = _crc_ccitt_update(crc16, *buffer);
    buffer++;
  }

  // make cksum high byte and low byte
  uint8_t crc_bytes[2];
  crc_bytes[0] = crc16 >> 8;
  crc_bytes[1] = crc16;

  if (!Wire.write(data, length)) return 1;
  if (!Wire.write(crc_bytes, 2)) return 1;
  if (Wire.endTransmission(true) != 0) return 1;
  return 0;
}

uint8_t TWI::readFrom(uint8_t *data, size_t length) {
  uint8_t counter = 0;
  uint32_t timeout;
  uint8_t *buffer = data;

  if (!Wire.requestFrom(addr_, length + 2, true)) {  // + 2 for the cksum
    // in case slave is not responding - return 0 (0 length of received data).
    return 0;
  }
  while (counter < length) {
    *data = Wire.read();
    data++;
    counter++;
  }

  uint16_t crc16    = 0xffff;
  uint16_t rx_cksum = (Wire.read() << 8) + Wire.read();
  for (uint8_t i = 0; i < length; i++) {
    crc16 = _crc_ccitt_update(crc16, *buffer);
    buffer++;
  }

  // check received CRC16
  if (crc16 != rx_cksum) {
    crc_errors_++;
    return 0;
  }

  return length;
}

void TWI::disable() {
  Wire.end();
}

void TWI::init(uint16_t clock_khz) {
  Wire.begin();
  Wire.setClock(clock_khz * 1000);
}

}  // namespace raise
}  // namespace dygma
}  // namespace device
}  // namespace kaleidoscope

#endif

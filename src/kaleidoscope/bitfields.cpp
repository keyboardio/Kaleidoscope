/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2018  Keyboard.io, Inc.
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

#include "Kaleidoscope.h"

#include "bitfields.h"

namespace kaleidoscope {
namespace bitfields {
namespace internal {

bool _BaseBitfield::isBitSetP(const void *bit_field, uint8_t raw_pos) {
  uint8_t byte_id = raw_pos >> 3;
  uint8_t bit_pos = raw_pos & 0x7;
  const uint8_t *bytes = reinterpret_cast<const uint8_t *>(bit_field);
  return bytes[byte_id] & (0x1 << bit_pos);
}

void _BaseBitfield::setBitP(void *bit_field, uint8_t raw_pos, uint8_t val) {
  uint8_t byte_id = raw_pos >> 3;
  uint8_t bit_pos = raw_pos & 0x7;
  uint8_t *bytes = reinterpret_cast<uint8_t *>(bit_field);
  if (val) {
    bytes[byte_id] |= (0x1 << bit_pos);
  } else {
    bytes[byte_id] &= ~(0x1 << bit_pos);
  }
}

bool _BaseBitfield::isBitSetPROGMEM_P(const void *bit_field, uint8_t raw_pos) {
  uint8_t byte_id = raw_pos >> 3;
  uint8_t bit_pos = raw_pos & 0x7;
  const uint8_t *bytes = reinterpret_cast<const uint8_t *>(bit_field);
  uint8_t byte = pgm_read_byte(&(bytes[byte_id]));
  return byte & (0x1 << bit_pos);
}

} // end namespace internal
} // end namespace bitfields
} // end namespace kaleidoscope

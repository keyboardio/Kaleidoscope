/* -*- mode: c++ -*-
 * Kaleidoscope-Qukeys -- Assign two keycodes to a single key
 * Copyright (C) 2017  Michael Richters
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

#include <Kaleidoscope.h>

// Helper functions for converting between separate (row,col)
// coordinates and a single-byte key number (addr). This works as long
// as the keyboard has fewer than 256 keys.
namespace kaleidoscope {
namespace addr {
KS_ROW_COL_FUNC inline uint8_t row(uint8_t key_addr) {
  return (key_addr / COLS);
}
KS_ROW_COL_FUNC inline uint8_t col(uint8_t key_addr) {
  return (key_addr % COLS);
}
KS_ROW_COL_FUNC inline uint8_t addr(uint8_t row, uint8_t col) {
  return ((row * COLS) + col);
}
KS_ROW_COL_FUNC inline void mask(uint8_t key_addr) {
  KeyboardHardware.maskKey(row(key_addr), col(key_addr));
}
KS_ROW_COL_FUNC inline void unmask(uint8_t key_addr) {
  KeyboardHardware.unMaskKey(row(key_addr), col(key_addr));
}
} // namespace addr {
} // namespace kaleidoscope {

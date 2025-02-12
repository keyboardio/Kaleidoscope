/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2019-2025 Keyboard.io, inc.
 *
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * Additional Permissions:
 * As an additional permission under Section 7 of the GNU General Public
 * License Version 3, you may link this software against a Vendor-provided
 * Hardware Specific Software Module under the terms of the MCU Vendor
 * Firmware Library Additional Permission Version 1.0.
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
namespace driver {
namespace led {
namespace color {

struct RGB {
  RGB(uint8_t r_, uint8_t g_, uint8_t b_)
    : r(r_), g(g_), b(b_) {}
  RGB() {}

  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct GRB {
  GRB(uint8_t r_, uint8_t g_, uint8_t b_)
    : g(g_), r(r_), b(b_) {}
  GRB() {}

  uint8_t g;
  uint8_t r;
  uint8_t b;
};

struct BGR {
  BGR(uint8_t r_, uint8_t g_, uint8_t b_)
    : b(b_), g(g_), r(r_) {}
  BGR() {}

  uint8_t b;
  uint8_t g;
  uint8_t r;
};

}  // namespace color
}  // namespace led
}  // namespace driver
}  // namespace kaleidoscope

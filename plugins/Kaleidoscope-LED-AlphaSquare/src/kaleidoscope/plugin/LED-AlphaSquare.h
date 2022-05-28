/* -*- mode: c++ -*-
 * Kaleidoscope-LED-AlphaSquare -- 4x4 pixel LED alphabet
 * Copyright (C) 2017  Keyboard.io, Inc
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

#include <stdint.h>  // for uint16_t, uint8_t

#include "kaleidoscope/KeyAddr.h"        // for KeyAddr
#include "kaleidoscope/device/device.h"  // for cRGB
#include "kaleidoscope/key_defs.h"       // for Key
#include "kaleidoscope/plugin.h"         // for Plugin

// clang-format off
#define SYM4x4(                                                   \
               p00, p01, p02, p03,                                \
               p10, p11, p12, p13,                                \
               p20, p21, p22, p23,                                \
               p30, p31, p32, p33)                                \
  (uint16_t) (                                                    \
              p00 <<  0 | p01 <<  1 | p02 <<  2 | p03 <<  3 |     \
              p10 <<  4 | p11 <<  5 | p12 <<  6 | p13 <<  7 |     \
              p20 <<  8 | p21 <<  9 | p22 << 10 | p23 << 11 |     \
              p30 << 12 | p31 << 13 | p32 << 14 | p33 << 15 )
// clang-format on

namespace kaleidoscope {
namespace plugin {
class AlphaSquare : public kaleidoscope::Plugin {
 public:
  static void display(Key key, KeyAddr key_addr, cRGB key_color);
  static void display(Key key, KeyAddr key_addr);
  static void display(Key key) {
    display(key, KeyAddr(0, 2));
  }
  static void display(Key key, uint8_t col) {
    display(key, KeyAddr(0, col));
  }

  static void display(uint16_t symbol, KeyAddr key_addr, cRGB key_color);
  static void display(uint16_t symbol, KeyAddr key_addr);
  static void display(uint16_t symbol) {
    display(symbol, KeyAddr(0, 2));
  }
  static void display(uint16_t symbol, uint8_t col) {
    display(symbol, KeyAddr(0, col));
  }

  static void clear(Key key, KeyAddr key_addr) {
    display(key, key_addr, {0, 0, 0});
  }
  static void clear(Key key, uint8_t col) {
    clear(key, KeyAddr(0, col));
  }
  static void clear(Key key) {
    clear(key, KeyAddr(0, 2));
  }

  static void clear(uint16_t symbol, KeyAddr key_addr) {
    display(symbol, key_addr, {0, 0, 0});
  }
  static void clear(uint16_t symbol, uint8_t col) {
    clear(symbol, KeyAddr(0, col));
  }
  static void clear(uint16_t symbol) {
    clear(symbol, KeyAddr(0, 2));
  }

  static bool isSymbolPart(Key key,
                           KeyAddr displayLedAddr,
                           KeyAddr key_addr);
  static bool isSymbolPart(uint16_t symbol,
                           KeyAddr displayLedAddr,
                           KeyAddr key_addr);

  static cRGB color;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::AlphaSquare AlphaSquare;

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

#include "kaleidoscope/plugin/LED-AlphaSquare.h"

#include <Arduino.h>  // for bitRead, pgm_read_word, PROGMEM
#include <stdint.h>   // for uint16_t, uint8_t

#include "kaleidoscope/KeyAddr.h"                          // for KeyAddr, MatrixAddr
#include "kaleidoscope/Runtime.h"                          // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"                    // for cRGB
#include "kaleidoscope/key_defs.h"                         // for Key, Key_A, Key_0
#include "kaleidoscope/plugin/LED-AlphaSquare/Font-4x4.h"  // for ALPHASQUARE_SYMBOL_0, ALPHASQU...
#include "kaleidoscope/plugin/LEDControl.h"                // for LEDControl

namespace kaleidoscope {
namespace plugin {

static const uint16_t alphabet[] PROGMEM = {
  ALPHASQUARE_SYMBOL_A,
  ALPHASQUARE_SYMBOL_B,
  ALPHASQUARE_SYMBOL_C,
  ALPHASQUARE_SYMBOL_D,
  ALPHASQUARE_SYMBOL_E,
  ALPHASQUARE_SYMBOL_F,
  ALPHASQUARE_SYMBOL_G,
  ALPHASQUARE_SYMBOL_H,
  ALPHASQUARE_SYMBOL_I,
  ALPHASQUARE_SYMBOL_J,
  ALPHASQUARE_SYMBOL_K,
  ALPHASQUARE_SYMBOL_L,
  ALPHASQUARE_SYMBOL_M,
  ALPHASQUARE_SYMBOL_N,
  ALPHASQUARE_SYMBOL_O,
  ALPHASQUARE_SYMBOL_P,
  ALPHASQUARE_SYMBOL_Q,
  ALPHASQUARE_SYMBOL_R,
  ALPHASQUARE_SYMBOL_S,
  ALPHASQUARE_SYMBOL_T,
  ALPHASQUARE_SYMBOL_U,
  ALPHASQUARE_SYMBOL_V,
  ALPHASQUARE_SYMBOL_W,
  ALPHASQUARE_SYMBOL_X,
  ALPHASQUARE_SYMBOL_Y,
  ALPHASQUARE_SYMBOL_Z,
  ALPHASQUARE_SYMBOL_1,
  ALPHASQUARE_SYMBOL_2,
  ALPHASQUARE_SYMBOL_3,
  ALPHASQUARE_SYMBOL_4,
  ALPHASQUARE_SYMBOL_5,
  ALPHASQUARE_SYMBOL_6,
  ALPHASQUARE_SYMBOL_7,
  ALPHASQUARE_SYMBOL_8,
  ALPHASQUARE_SYMBOL_9,
  ALPHASQUARE_SYMBOL_0};


cRGB AlphaSquare::color = {0x80, 0x80, 0x80};

void AlphaSquare::display(Key key, KeyAddr key_addr, cRGB key_color) {
  if (!Runtime.has_leds)
    return;

  if (key < Key_A || key > Key_0)
    return;

  uint8_t index   = key.getKeyCode() - Key_A.getKeyCode();
  uint16_t symbol = pgm_read_word(&alphabet[index]);

  display(symbol, key_addr, key_color);
}

void AlphaSquare::display(Key key, KeyAddr key_addr) {
  display(key, key_addr, color);
}

void AlphaSquare::display(uint16_t symbol, KeyAddr key_addr, cRGB key_color) {
  if (!Runtime.has_leds)
    return;

  for (uint8_t r = 0; r < 4; r++) {
    for (uint8_t c = 0; c < 4; c++) {
      uint8_t pixel = bitRead(symbol, r * 4 + c);
      if (!pixel)
        continue;

      KeyAddr shifted_addr = key_addr.shifted(r, c);

      ::LEDControl.setCrgbAt(shifted_addr, key_color);
    }
  }

  ::LEDControl.syncLeds();
}

void AlphaSquare::display(uint16_t symbol, KeyAddr key_addr) {
  display(symbol, key_addr, color);
}

bool AlphaSquare::isSymbolPart(Key key,
                               KeyAddr displayLedAddr,
                               KeyAddr key_addr) {
  if (!Runtime.has_leds)
    return false;

  if (key < Key_A || key > Key_0)
    return false;

  uint8_t index   = key.getKeyCode() - Key_A.getKeyCode();
  uint16_t symbol = pgm_read_word(&alphabet[index]);

  return isSymbolPart(symbol, displayLedAddr, key_addr);
}

bool AlphaSquare::isSymbolPart(uint16_t symbol,
                               KeyAddr displayLedAddr,
                               KeyAddr key_addr) {
  if (!Runtime.has_leds)
    return false;

  for (uint8_t r = 0; r < 4; r++) {
    for (uint8_t c = 0; c < 4; c++) {
      uint8_t pixel = bitRead(symbol, r * 4 + c);

      KeyAddr addr_shifted = displayLedAddr.shifted(r, c);
      if (addr_shifted == key_addr)
        return !!pixel;
    }
  }

  return false;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::AlphaSquare AlphaSquare;

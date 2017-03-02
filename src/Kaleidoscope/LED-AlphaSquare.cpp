/* -*- mode: c++ -*-
 * Kaleidoscope-LED-AlphaSquare -- 4x4 pixel LED alphabet
 * Copyright (C) 2017  Gergely Nagy
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

#include <Kaleidoscope-LED-AlphaSquare.h>

namespace KaleidoscopePlugins {

  static const uint16_t alphabet[] PROGMEM = {
    SYM4x4(1, 1, 1, 1,
           1, 0, 0, 1,
           1, 1, 1, 1,
           1, 0, 0, 1), // A
    SYM4x4(1, 1, 1, 1,
           1, 0, 1, 1,
           1, 1, 0, 1,
           1, 1, 1, 1), // B
    SYM4x4(1, 1, 1, 1,
           1, 0, 0, 0,
           1, 0, 0, 0,
           1, 1, 1, 1), // C
    SYM4x4(1, 1, 1, 0,
           1, 0, 0, 1,
           1, 0, 0, 1,
           1, 1, 1, 0), // D
    SYM4x4(1, 1, 1, 1,
           1, 1, 0, 0,
           1, 0, 0, 0,
           1, 1, 1, 1), // E
    SYM4x4(1, 1, 1, 1,
           1, 0, 0, 0,
           1, 1, 1, 0,
           1, 0, 0, 0), // F
    SYM4x4(1, 1, 1, 0,
           1, 0, 0, 0,
           1, 0, 0, 1,
           1, 1, 1, 1), // G
    SYM4x4(1, 0, 0, 1,
           1, 1, 1, 1,
           1, 0, 0, 1,
           1, 0, 0, 1), // H
    SYM4x4(1, 1, 1, 1,
           0, 1, 1, 0,
           0, 1, 1, 0,
           1, 1, 1, 1), // I
    SYM4x4(1, 1, 1, 1,
           0, 0, 0, 1,
           1, 0, 0, 1,
           0, 1, 1, 0), // J
    SYM4x4(1, 0, 0, 1,
           1, 1, 0, 0,
           1, 1, 0, 0,
           1, 0, 1, 1), // K
    SYM4x4(1, 0, 0, 0,
           1, 0, 0, 0,
           1, 0, 0, 0,
           1, 1, 1, 1), // L
    SYM4x4(1, 0, 1, 1,
           1, 1, 1, 1,
           1, 1, 0, 1,
           1, 0, 0, 1), // M
    SYM4x4(1, 0, 0, 1,
           1, 1, 0, 1,
           1, 0, 1, 1,
           1, 0, 0, 1), // N
    SYM4x4(1, 1, 1, 1,
           1, 0, 0, 1,
           1, 0, 0, 1,
           1, 1, 1, 1), // O
    SYM4x4(1, 1, 1, 1,
           1, 0, 0, 1,
           1, 1, 1, 1,
           1, 0, 0, 0), // P
    SYM4x4(1, 1, 1, 1,
           1, 0, 0, 1,
           1, 0, 1, 1,
           1, 1, 1, 1), // Q
    SYM4x4(1, 1, 1, 1,
           1, 0, 0, 1,
           1, 1, 1, 0,
           1, 0, 1, 1), // R
    SYM4x4(1, 1, 1, 1,
           1, 1, 0, 0,
           0, 0, 1, 1,
           1, 1, 1, 1), // S
    SYM4x4(1, 1, 1, 1,
           0, 1, 1, 0,
           0, 1, 1, 0,
           0, 1, 1, 0), // T
    SYM4x4(1, 0, 0, 1,
           1, 0, 0, 1,
           1, 0, 0, 1,
           1, 1, 1, 1), // U
    SYM4x4(1, 0, 0, 1,
           1, 0, 0, 1,
           1, 0, 0, 1,
           0, 1, 1, 0), // V
    SYM4x4(1, 0, 0, 1,
           1, 0, 1, 1,
           1, 1, 1, 1,
           1, 0, 1, 1), // W
    SYM4x4(1, 0, 0, 1,
           0, 1, 1, 0,
           0, 1, 1, 0,
           1, 0, 0, 1), // X
    SYM4x4(1, 0, 0, 1,
           1, 1, 1, 1,
           0, 1, 1, 0,
           0, 1, 1, 0), // Y
    SYM4x4(1, 1, 1, 1,
           0, 0, 1, 1,
           1, 1, 0, 0,
           1, 1, 1, 1), // Z
    // ---------------------
    SYM4x4(0, 1, 1, 0,
           1, 0, 1, 0,
           0, 0, 1, 0,
           1, 1, 1, 1), // 1
    SYM4x4(0, 1, 1, 0,
           1, 0, 0, 1,
           0, 0, 1, 0,
           1, 1, 0, 1), // 2
    SYM4x4(1, 1, 1, 1,
           0, 0, 1, 1,
           0, 0, 0, 1,
           1, 1, 1, 1), // 3
    SYM4x4(1, 0, 0, 1,
           1, 1, 1, 1,
           0, 0, 0, 1,
           0, 0, 0, 1), // 4
    SYM4x4(1, 1, 1, 1,
           1, 0, 0, 0,
           0, 1, 1, 1,
           1, 1, 1, 1), // 5
    SYM4x4(0, 1, 1, 0,
           1, 0, 0, 0,
           1, 1, 1, 1,
           1, 1, 1, 1), // 6
    SYM4x4(1, 1, 1, 1,
           0, 0, 0, 1,
           0, 0, 1, 0,
           0, 1, 0, 0), // 7
    SYM4x4(1, 1, 1, 0,
           1, 0, 1, 1,
           1, 1, 0, 1,
           0, 1, 1, 1), // 8
    SYM4x4(1, 1, 1, 1,
           1, 0, 0, 1,
           1, 1, 1, 1,
           0, 0, 0, 1), // 9
    SYM4x4(0, 1, 1, 0,
           1, 0, 0, 1,
           1, 0, 0, 1,
           0, 1, 1, 0), // 0
  };

  cRGB AlphaSquare::color = {0x80, 0x80, 0x80};

  AlphaSquare::AlphaSquare (void) {
  }

  void
  AlphaSquare::begin (void) {
  }

  void
  AlphaSquare::display (Key key, uint8_t row, uint8_t col, cRGB keyColor) {
    if (key < Key_A || key > Key_0)
      return;

    uint8_t index = key.keyCode - Key_A.keyCode;
    uint16_t symbol = pgm_read_word (&alphabet[index]);

    display (symbol, row, col, keyColor);
  }

  void
  AlphaSquare::display (Key key, uint8_t row, uint8_t col) {
    display (key, row, col, color);
  }

  void
  AlphaSquare::display (uint16_t symbol, uint8_t row, uint8_t col, cRGB keyColor) {
    for (uint8_t r = 0; r < 4; r++) {
      for (uint8_t c = 0; c < 4; c++) {
        uint8_t pixel = bitRead (symbol, r * 4 + c);
        if (!pixel)
          continue;

        LEDControl.led_set_crgb_at (row + r, col + c, keyColor);
      }
    }

    LEDControl.led_sync ();
  }

  void
  AlphaSquare::display (uint16_t symbol, uint8_t row, uint8_t col) {
    display (symbol, row, col, color);
  }
};

KaleidoscopePlugins::AlphaSquare AlphaSquare;

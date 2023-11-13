/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-EZ-ErgoDox -- ErgoDox hardware support for Kaleidoscope
 * Copyright (C) 2018, 2019  Keyboard.io, Inc
 *
 * Based on QMK (commit 8cdb4a915)
 *  (C) Jack Humbert, Erez Zukerman, Oleg Kostyuk
 * Original QMK sources:
 *  - keyboards/ergodox_ez/ergodox_ez.c
 *  - keyboards/ergodox_ez/ergodox_ez.h
 *  - keyboards/ergodox_ez/matrix.c
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

#ifdef ARDUINO_AVR_ERGODOX

#include <Arduino.h>

struct cRGB {
  uint8_t r, g, b;
};

#define CRGB(r, g, b) \
  (cRGB) {            \
    b, g, r           \
  }

#include "kaleidoscope/device/ATmega32U4Keyboard.h"
#include "kaleidoscope/driver/bootloader/avr/HalfKay.h"
#include "kaleidoscope/driver/keyscanner/Base.h"
#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
#include "kaleidoscope/device/ez/ErgoDox/ErgoDoxScanner.h"
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

namespace kaleidoscope {
namespace device {
namespace ez {

struct ErgoDoxProps : public kaleidoscope::device::ATmega32U4KeyboardProps {
  struct KeyScannerProps : kaleidoscope::driver::keyscanner::BaseProps {
    static constexpr uint8_t matrix_rows    = 14;
    static constexpr uint8_t matrix_columns = 6;
    typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;
  };
  typedef kaleidoscope::driver::bootloader::avr::HalfKay Bootloader;
  static constexpr const char *short_name = "ErgoDox-EZ";
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
class ErgoDox : public kaleidoscope::device::ATmega32U4Keyboard<ErgoDoxProps> {
 public:
  void scanMatrix();
  void readMatrix();
  void actOnMatrixScan();
  void setup();

  bool isKeyswitchPressed(KeyAddr key_addr);
  bool isKeyswitchPressed(uint8_t keyIndex);
  uint8_t pressedKeyswitchCount();

  bool wasKeyswitchPressed(KeyAddr key_addr);

  bool wasKeyswitchPressed(uint8_t keyIndex);
  uint8_t previousPressedKeyswitchCount();

  // ErgoDox-specific stuff
  void setStatusLED(uint8_t led, bool state = true);
  void setStatusLEDBrightness(uint8_t led, uint8_t brightness);

  uint8_t debounce = 5;

 private:
  ErgoDoxScanner scanner_;
  uint8_t previousKeyState_[matrix_rows];  // NOLINT(runtime/arrays)
  uint8_t keyState_[matrix_rows];          // NOLINT(runtime/arrays)
  uint8_t debounce_matrix_[matrix_rows][matrix_columns];

  uint8_t debounceMaskForRow(uint8_t row);
  void debounceRow(uint8_t change, uint8_t row);
  void readMatrixRow(uint8_t row);
};
#else   // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
class ErgoDox;
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

// clang-format off

#define PER_KEY_DATA_STACKED(dflt,                                    \
    /* left hand, spatial positions */                                  \
    r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6,                           \
    r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6,                           \
    r2c0, r2c1, r2c2, r2c3, r2c4, r2c5,                                 \
    r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6,                           \
    r4c0, r4c1, r4c2, r4c3, r4c4,                                       \
                            r5c5, r5c6,                                 \
                                  r5c4,                                 \
                      r5c3, r5c2, r5c1,                                 \
                                                                        \
    /* right hand, spatial positions */                                 \
    r0c7,  r0c8,  r0c9,  r0c10, r0c11, r0c12, r0c13,                    \
    r1c7,  r1c8,  r1c9,  r1c10, r1c11, r1c12, r1c13,                    \
           r2c8,  r2c9,  r2c10, r2c11, r2c12, r2c13,                    \
    r3c7,  r3c8,  r3c9,  r3c10, r3c11, r3c12, r3c13,                    \
    r4c9,  r4c10, r4c11, r4c12, r4c13,                                  \
    r5c7,  r5c8,                                                        \
    r5c9,                                                               \
    r5c12, r5c11, r5c10 )                                               \
                                                                        \
  /* matrix positions */                                                \
                                                                        \
    r0c0, r1c0, r2c0, r3c0, r4c0, dflt,                                 \
    r0c1, r1c1, r2c1, r3c1, r4c1, r5c1,                                 \
    r0c2, r1c2, r2c2, r3c2, r4c2, r5c2,                                 \
    r0c3, r1c3, r2c3, r3c3, r4c3, r5c3,                                 \
    r0c4, r1c4, r2c4, r3c4, r4c4, r5c4,                                 \
    r0c5, r1c5, r2c5, r3c5, dflt, r5c5,                                 \
    r0c6, r1c6, dflt, r3c6, dflt, r5c6,                                 \
                                                                        \
    r0c7,  r1c7,  dflt,  r3c7,  dflt,  r5c7,                            \
    r0c8,  r1c8,  r2c8,  r3c8,  dflt,  r5c8,                            \
    r0c9,  r1c9,  r2c9,  r3c9,  r4c9,  r5c9,                            \
    r0c10, r1c10, r2c10, r3c10, r4c10, r5c10,                           \
    r0c11, r1c11, r2c11, r3c11, r4c11, r5c11,                           \
    r0c12, r1c12, r2c12, r3c12, r4c12, r5c12,                           \
    r0c13, r1c13, r2c13, r3c13, r4c13, dflt

}  // namespace ez
}  // namespace device

EXPORT_DEVICE(kaleidoscope::device::ez::ErgoDox)

}  // namespace kaleidoscope

#endif

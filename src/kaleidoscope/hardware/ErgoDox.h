/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-EZ-ErgoDox -- ErgoDox hardware support for Kaleidoscope
 * Copyright (C) 2018  Gergely Nagy
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

#include <Arduino.h>

#include "ErgoDoxScanner.h"

#define HARDWARE_IMPLEMENTATION kaleidoscope::hardware::ErgoDox
#include "Kaleidoscope-HIDAdaptor-KeyboardioHID.h"

#include "macro_helpers.h"

struct cRGB {
  uint8_t r, g, b;
};

#define COLS 6
#define ROWS 14
#define LED_COUNT 0

#define CRGB(r,g,b) (cRGB){b, g, r}

namespace kaleidoscope {
namespace hardware {

class ErgoDox {
 public:
  ErgoDox(void) {};

  void syncLeds(void) {};
  void setCrgbAt(byte row, byte col, cRGB color) {};
  void setCrgbAt(uint8_t i, cRGB crgb) {};
  cRGB getCrgbAt(uint8_t i) {
    return CRGB(0, 0, 0);
  };

  void scanMatrix(void);
  void readMatrix(void);
  void actOnMatrixScan(void);
  void setup();

  /** Detaching from / attaching to the host.
   *
   * These two functions should detach the device from (or attach it to) the
   * host, preferably without rebooting the device. Their purpose is to allow
   * one to do some configuration inbetween, so the re-attach happens with
   * different properties. The device remains powered between these operations,
   * only the connection to the host gets severed.
   */
  void detachFromHost();
  void attachToHost();

  /* Key masking
   * -----------
   *
   * There are situations when one wants to ignore key events for a while, and
   * mask them out. These functions help do that. In isolation, they do nothing,
   * plugins and the core firmware is expected to make use of these.
   *
   * See `handleKeyswitchEvent` in the Kaleidoscope sources for a use-case.
   */
  void maskKey(byte row, byte col);
  void unMaskKey(byte row, byte col);
  bool isKeyMasked(byte row, byte col);

  /** Key switch state
   *
   * These two methods return the state of the keyswitch at any given position,
   * regardless of which half they are on. This is a hardware-agnostic access to
   * the key switch states.
   *
   * The first variant requires a row and a column, the second an index, as
   * returned by `keyIndex`.
   */
  uint8_t getKeyswitchStateAtPosition(byte row, byte col);
  uint8_t getKeyswitchStateAtPosition(uint8_t keyIndex);

  // ErgoDox-specific stuff
  void setStatusLED(uint8_t led, bool state = true);
  void setStatusLEDBrightness(uint8_t led, uint8_t brightness);

  void resetDevice();

  static uint8_t debounce;

 private:
  static ErgoDoxScanner scanner_;
  static uint8_t previousKeyState_[ROWS];
  static uint8_t keyState_[ROWS];
  static uint8_t masks_[ROWS];
  static uint8_t debounce_matrix_[ROWS][COLS];

  static uint8_t debounceMaskForRow(uint8_t row);
  static void debounceRow(uint8_t change, uint8_t row);
  static void readMatrixRow(uint8_t row);
};

#define KEYMAP_STACKED(                                                 \
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
  {                                                                     \
    { r0c0, r1c0, r2c0, r3c0, r4c0, XXX  },                             \
    { r0c1, r1c1, r2c1, r3c1, r4c1, r5c1 },                             \
    { r0c2, r1c2, r2c2, r3c2, r4c2, r5c2 },                             \
    { r0c3, r1c3, r2c3, r3c3, r4c3, r5c3 },                             \
    { r0c4, r1c4, r2c4, r3c4, r4c4, r5c4 },                             \
    { r0c5, r1c5, r2c5, r3c5, XXX,  r5c5 },                             \
    { r0c6, r1c6, XXX,  r3c6, XXX,  r5c6 },                             \
                                                                        \
    { r0c7,  r1c7,  XXX,   r3c7,  XXX,   r5c7  },                       \
    { r0c8,  r1c8,  r2c8,  r3c8,  XXX,   r5c8  },                       \
    { r0c9,  r1c9,  r2c9,  r3c9,  r4c9,  r5c9  },                       \
    { r0c10, r1c10, r2c10, r3c10, r4c10, r5c10 },                       \
    { r0c11, r1c11, r2c11, r3c11, r4c11, r5c11 },                       \
    { r0c12, r1c12, r2c12, r3c12, r4c12, r5c12 },                       \
    { r0c13, r1c13, r2c13, r3c13, r4c13, XXX   }                        \
  }
}
}

extern kaleidoscope::hardware::ErgoDox &ErgoDox;

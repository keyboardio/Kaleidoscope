/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-EZ-ErgoDox -- ErgoDox hardware support for Kaleidoscope
 * Copyright (C) 2018  Keyboard.io, Inc
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

#include "kaleidoscope/hardware/ErgoDox/ErgoDoxScanner.h"

#define HARDWARE_IMPLEMENTATION kaleidoscope::hardware::ErgoDox
#include "Kaleidoscope-HIDAdaptor-KeyboardioHID.h"

#include "kaleidoscope/macro_helpers.h"

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
  ErgoDox(void) {}

  void syncLeds(void) {}
  void setCrgbAt(byte row, byte col, cRGB color) {}
  void setCrgbAt(uint8_t i, cRGB crgb) {}
  cRGB getCrgbAt(uint8_t i) {
    return CRGB(0, 0, 0);
  }

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

  /** Key switch states
   *
   * These methods offer a way to peek at the key switch states, for those cases
   * where we need to deal with the state closest to the hardware. Some methods
   * offer a way to check if a key is pressed, others return the number of
   * pressed keys.
   */
  /**
   * Check if a key is pressed at a given position.
   *
   * @param row is the row the key is located at in the matrix.
   * @param col is the column the key is located at in the matrix.
   *
   * @returns true if the key is pressed, false otherwise.
   */
  bool isKeyswitchPressed(byte row, byte col);
  /**
   * Check if a key is pressed at a given position.
   *
   * @param keyIndex is the key index, as calculated by `keyIndex`.
   *
   * @note Key indexes start at 1, not 0!
   *
   * @returns true if the key is pressed, false otherwise.
   */
  bool isKeyswitchPressed(uint8_t keyIndex);
  /**
   * Check the number of key switches currently pressed.
   *
   * @returns the number of keys pressed.
   */
  uint8_t pressedKeyswitchCount();

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

/* To be used by the hardware implementations, `keyIndex` tells us the index of
 * a key, from which we can figure out the row and column as needed. The index
 * starts at one, so that plugins that work with a list of key indexes can use
 * zero as a sentinel. This is important, because when we initialize arrays with
 * fewer elements than the declared array size, the remaining elements will be
 * zero. We can use this to avoid having to explicitly add a sentinel in
 * user-facing code.
 */
constexpr byte keyIndex(byte row, byte col) {
  return row * COLS + col + 1;
}

constexpr byte R0C0  = keyIndex(0, 0);
constexpr byte R0C1  = keyIndex(0, 1);
constexpr byte R0C2  = keyIndex(0, 2);
constexpr byte R0C3  = keyIndex(0, 3);
constexpr byte R0C4  = keyIndex(0, 4);
constexpr byte R0C5  = keyIndex(0, 5);
constexpr byte R0C6  = keyIndex(0, 6);
constexpr byte R0C7  = keyIndex(0, 7);
constexpr byte R0C8  = keyIndex(0, 8);
constexpr byte R0C9  = keyIndex(0, 9);
constexpr byte R0C10 = keyIndex(0, 10);
constexpr byte R0C11 = keyIndex(0, 11);
constexpr byte R0C12 = keyIndex(0, 12);
constexpr byte R0C13 = keyIndex(0, 13);

constexpr byte R1C0  = keyIndex(1, 0);
constexpr byte R1C1  = keyIndex(1, 1);
constexpr byte R1C2  = keyIndex(1, 2);
constexpr byte R1C3  = keyIndex(1, 3);
constexpr byte R1C4  = keyIndex(1, 4);
constexpr byte R1C5  = keyIndex(1, 5);
constexpr byte R1C6  = keyIndex(1, 6);
constexpr byte R1C7  = keyIndex(1, 7);
constexpr byte R1C8  = keyIndex(1, 8);
constexpr byte R1C9  = keyIndex(1, 9);
constexpr byte R1C10 = keyIndex(1, 10);
constexpr byte R1C11 = keyIndex(1, 11);
constexpr byte R1C12 = keyIndex(1, 12);
constexpr byte R1C13 = keyIndex(1, 13);

constexpr byte R2C0  = keyIndex(2, 0);
constexpr byte R2C1  = keyIndex(2, 1);
constexpr byte R2C2  = keyIndex(2, 2);
constexpr byte R2C3  = keyIndex(2, 3);
constexpr byte R2C4  = keyIndex(2, 4);
constexpr byte R2C5  = keyIndex(2, 5);
constexpr byte R2C6  = keyIndex(2, 6);
constexpr byte R2C7  = keyIndex(2, 7);
constexpr byte R2C8  = keyIndex(2, 8);
constexpr byte R2C9  = keyIndex(2, 9);
constexpr byte R2C10 = keyIndex(2, 10);
constexpr byte R2C11 = keyIndex(2, 11);
constexpr byte R2C12 = keyIndex(2, 12);
constexpr byte R2C13 = keyIndex(2, 13);

constexpr byte R3C0  = keyIndex(3, 0);
constexpr byte R3C1  = keyIndex(3, 1);
constexpr byte R3C2  = keyIndex(3, 2);
constexpr byte R3C3  = keyIndex(3, 3);
constexpr byte R3C4  = keyIndex(3, 4);
constexpr byte R3C5  = keyIndex(3, 5);
constexpr byte R3C6  = keyIndex(3, 6);
constexpr byte R3C7  = keyIndex(3, 7);
constexpr byte R3C8  = keyIndex(3, 8);
constexpr byte R3C9  = keyIndex(3, 9);
constexpr byte R3C10 = keyIndex(3, 10);
constexpr byte R3C11 = keyIndex(3, 11);
constexpr byte R3C12 = keyIndex(3, 12);
constexpr byte R3C13 = keyIndex(3, 13);

constexpr byte R4C0  = keyIndex(4, 0);
constexpr byte R4C1  = keyIndex(4, 1);
constexpr byte R4C2  = keyIndex(4, 2);
constexpr byte R4C3  = keyIndex(4, 3);
constexpr byte R4C4  = keyIndex(4, 4);
constexpr byte R4C5  = keyIndex(4, 5);
constexpr byte R4C6  = keyIndex(4, 6);
constexpr byte R4C7  = keyIndex(4, 7);
constexpr byte R4C8  = keyIndex(4, 8);
constexpr byte R4C9  = keyIndex(4, 9);
constexpr byte R4C10 = keyIndex(4, 10);
constexpr byte R4C11 = keyIndex(4, 11);
constexpr byte R4C12 = keyIndex(4, 12);
constexpr byte R4C13 = keyIndex(4, 13);

constexpr byte R5C0  = keyIndex(5, 0);
constexpr byte R5C1  = keyIndex(5, 1);
constexpr byte R5C2  = keyIndex(5, 2);
constexpr byte R5C3  = keyIndex(5, 3);
constexpr byte R5C4  = keyIndex(5, 4);
constexpr byte R5C5  = keyIndex(5, 5);
constexpr byte R5C6  = keyIndex(5, 6);
constexpr byte R5C7  = keyIndex(5, 7);
constexpr byte R5C8  = keyIndex(5, 8);
constexpr byte R5C9  = keyIndex(5, 9);
constexpr byte R5C10 = keyIndex(5, 10);
constexpr byte R5C11 = keyIndex(5, 11);
constexpr byte R5C12 = keyIndex(5, 12);
constexpr byte R5C13 = keyIndex(5, 13);

extern kaleidoscope::hardware::ErgoDox &ErgoDox;

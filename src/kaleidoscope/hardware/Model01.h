/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Model01 -- Keyboard.io Model01 hardware support for Kaleidoscope
 * Copyright (C) 2017-2018  Keyboard.io, Inc
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

#include <Arduino.h>

#define HARDWARE_IMPLEMENTATION kaleidoscope::hardware::Model01
#include "Kaleidoscope-HIDAdaptor-KeyboardioHID.h"
#include "KeyboardioScanner.h"

#include "kaleidoscope/macro_helpers.h"

#define COLS 16
#define ROWS 4

#define CRGB(r,g,b) (cRGB){b, g, r}

namespace kaleidoscope {
namespace hardware {

class Model01 {
 public:
  Model01(void);
  void syncLeds(void);
  void setCrgbAt(byte row, byte col, cRGB color);
  void setCrgbAt(uint8_t i, cRGB crgb);
  cRGB getCrgbAt(uint8_t i);
  uint8_t getLedIndex(byte row, byte col);

  void scanMatrix(void);
  void readMatrix(void);
  void actOnMatrixScan(void);
  void setup();
  void rebootBootloader();


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

  /* These public functions are things supported by the Model 01, but
   * aren't necessarily part of the Kaleidoscope API
   */
  void enableHighPowerLeds(void);
  void enableScannerPower(void);
  void setKeyscanInterval(uint8_t interval);
  boolean ledPowerFault(void);

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
  void maskHeldKeys(void);

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

  keydata_t leftHandState;
  keydata_t rightHandState;
  keydata_t previousLeftHandState;
  keydata_t previousRightHandState;

 private:
  static void actOnHalfRow(byte row, byte colState, byte colPrevState, byte startPos);

  static bool isLEDChanged;
  static KeyboardioScanner leftHand;
  static KeyboardioScanner rightHand;

  static keydata_t leftHandMask;
  static keydata_t rightHandMask;
};

}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

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
constexpr byte R1C0  = keyIndex(1, 0);
constexpr byte R1C1  = keyIndex(1, 1);
constexpr byte R1C2  = keyIndex(1, 2);
constexpr byte R1C3  = keyIndex(1, 3);
constexpr byte R1C4  = keyIndex(1, 4);
constexpr byte R1C5  = keyIndex(1, 5);
constexpr byte R1C6  = keyIndex(1, 6);
constexpr byte R1C7  = keyIndex(1, 7);
constexpr byte R2C0  = keyIndex(2, 0);
constexpr byte R2C1  = keyIndex(2, 1);
constexpr byte R2C2  = keyIndex(2, 2);
constexpr byte R2C3  = keyIndex(2, 3);
constexpr byte R2C4  = keyIndex(2, 4);
constexpr byte R2C5  = keyIndex(2, 5);
constexpr byte R2C6  = keyIndex(2, 6);
constexpr byte R2C7  = keyIndex(2, 7);
constexpr byte R3C0  = keyIndex(3, 0);
constexpr byte R3C1  = keyIndex(3, 1);
constexpr byte R3C2  = keyIndex(3, 2);
constexpr byte R3C3  = keyIndex(3, 3);
constexpr byte R3C4  = keyIndex(3, 4);
constexpr byte R3C5  = keyIndex(3, 5);
constexpr byte R3C6  = keyIndex(3, 6);
constexpr byte R3C7  = keyIndex(3, 7);

constexpr byte R0C8  = keyIndex(0, 8);
constexpr byte R0C9  = keyIndex(0, 9);
constexpr byte R0C10 = keyIndex(0, 10);
constexpr byte R0C11 = keyIndex(0, 11);
constexpr byte R0C12 = keyIndex(0, 12);
constexpr byte R0C13 = keyIndex(0, 13);
constexpr byte R0C14 = keyIndex(0, 15);
constexpr byte R0C15 = keyIndex(0, 16);
constexpr byte R1C8  = keyIndex(1, 8);
constexpr byte R1C9  = keyIndex(1, 9);
constexpr byte R1C10 = keyIndex(1, 10);
constexpr byte R1C11 = keyIndex(1, 11);
constexpr byte R1C12 = keyIndex(1, 12);
constexpr byte R1C13 = keyIndex(1, 13);
constexpr byte R1C14 = keyIndex(1, 14);
constexpr byte R1C15 = keyIndex(1, 15);
constexpr byte R2C8  = keyIndex(2, 8);
constexpr byte R2C9  = keyIndex(2, 9);
constexpr byte R2C10 = keyIndex(2, 10);
constexpr byte R2C11 = keyIndex(2, 11);
constexpr byte R2C12 = keyIndex(2, 12);
constexpr byte R2C13 = keyIndex(2, 13);
constexpr byte R2C14 = keyIndex(2, 14);
constexpr byte R2C15 = keyIndex(2, 15);
constexpr byte R3C8  = keyIndex(3, 8);
constexpr byte R3C9  = keyIndex(3, 9);
constexpr byte R3C10 = keyIndex(3, 10);
constexpr byte R3C11 = keyIndex(3, 11);
constexpr byte R3C12 = keyIndex(3, 12);
constexpr byte R3C13 = keyIndex(3, 13);
constexpr byte R3C14 = keyIndex(3, 14);
constexpr byte R3C15 = keyIndex(3, 15);


#define LED_COUNT 64


#define LED_PGDN 0
#define LED_PGUP 1
#define LED_BACKTICK 2
#define LED_PROG 3
#define LED_1 4
#define LED_Q 5
#define LED_A 6
#define LED_Z 7
#define LED_X 8
#define LED_S 9
#define LED_W 10
#define LED_2 11
#define LED_3 12
#define LED_E 13
#define LED_D 14
#define LED_C 15
#define LED_V 16
#define LED_F 17
#define LED_R 18
#define LED_4 19
#define LED_5 20
#define LED_T 21
#define LED_G 22
#define LED_B 23
#define LED_ESC 24
#define LED_TAB 25
#define LED_LED 26
#define LED_L_CTRL 27
#define LED_BKSP 28
#define LED_CMD 29
#define LED_L_SHIFT 30
#define LED_L_FN 31
#define LED_R_FN 32
#define LED_R_SHIFT 33
#define LED_ALT 34
#define LED_SPACE 35
#define LED_R_CTRL 36
#define LED_ANY 37
#define LED_RETURN 38
#define LED_BUTTERFLY 39
#define LED_N 40
#define LED_H 41
#define LED_Y 42
#define LED_6 43
#define LED_7 44
#define LED_U 45
#define LED_J 46
#define LED_M 47
#define LED_COMMA 48
#define LED_K 49
#define LED_I 50
#define LED_8 51
#define LED_9 52
#define LED_O 53
#define LED_L 54
#define LED_PERIOD 55
#define LED_SLASH 56
#define LED_SEMICOLON 57
#define LED_P 58
#define LED_0 59
#define LED_NUM 60
#define LED_EQUALS 61
#define LED_APOSTROPHE 62
#define LED_MINUS 63

#endif /* DOXYGEN_SHOULD_SKIP_THIS */


#define KEYMAP_STACKED(                                                 \
               r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6,                \
               r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6,                \
               r2c0, r2c1, r2c2, r2c3, r2c4, r2c5,                      \
               r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r2c6,                \
               r0c7, r1c7, r2c7, r3c7,                                  \
               r3c6,                                                    \
                                                                        \
               r0c9,  r0c10, r0c11, r0c12, r0c13, r0c14, r0c15,         \
               r1c9,  r1c10, r1c11, r1c12, r1c13, r1c14, r1c15,         \
                      r2c10, r2c11, r2c12, r2c13, r2c14, r2c15,         \
               r2c9,  r3c10, r3c11, r3c12, r3c13, r3c14, r3c15,         \
               r3c8,  r2c8,  r1c8, r0c8,                                \
               r3c9, ...)                                               \
  {                                                                     \
    {r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6, r0c7, r0c8, r0c9, r0c10, r0c11, r0c12, r0c13, r0c14, r0c15}, \
    {r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1c10, r1c11, r1c12, r1c13, r1c14, r1c15}, \
    {r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, r2c6, r2c7, r2c8, r2c9, r2c10, r2c11, r2c12, r2c13, r2c14, r2c15}, \
    {r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6, r3c7, r3c8, r3c9, r3c10, r3c11, r3c12, r3c13, r3c14, RESTRICT_ARGS_COUNT((r3c15), 64, KEYMAP_STACKED, ##__VA_ARGS__)}, \
  }

#define KEYMAP(                                                                                     \
  r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6,        r0c9,  r0c10, r0c11, r0c12, r0c13, r0c14, r0c15, \
  r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6,        r1c9,  r1c10, r1c11, r1c12, r1c13, r1c14, r1c15, \
  r2c0, r2c1, r2c2, r2c3, r2c4, r2c5,                     r2c10, r2c11, r2c12, r2c13, r2c14, r2c15, \
  r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r2c6,        r2c9,  r3c10, r3c11, r3c12, r3c13, r3c14, r3c15, \
              r0c7, r1c7, r2c7, r3c7,                             r3c8,  r2c8,  r1c8, r0c8,         \
                          r3c6,                                          r3c9, ...)                      \
  {                                                                                                 \
    {r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6, r0c7, r0c8, r0c9, r0c10, r0c11, r0c12, r0c13, r0c14, r0c15}, \
    {r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1c10, r1c11, r1c12, r1c13, r1c14, r1c15}, \
    {r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, r2c6, r2c7, r2c8, r2c9, r2c10, r2c11, r2c12, r2c13, r2c14, r2c15}, \
    {r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6, r3c7, r3c8, r3c9, r3c10, r3c11, r3c12, r3c13, r3c14, RESTRICT_ARGS_COUNT((r3c15), 64, KEYMAP, ##__VA_ARGS__)}, \
  }

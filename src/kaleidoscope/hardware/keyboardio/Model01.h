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

#ifdef ARDUINO_AVR_MODEL01

#include <Arduino.h>

#define HARDWARE_IMPLEMENTATION kaleidoscope::hardware::keyboardio::Model01
#include "Kaleidoscope-HIDAdaptor-KeyboardioHID.h"
#include "KeyboardioScanner.h"

#include "kaleidoscope/macro_helpers.h"
#include "kaleidoscope_internal/deprecations.h"

#define CRGB(r,g,b) (cRGB){b, g, r}

#include "kaleidoscope/Hardware.h"
#include "kaleidoscope/MatrixAddr.h"
#include "kaleidoscope/driver/bootloader/avr/Caterina.h"

#define _DEPRECATED_MESSAGE_MODEL01_REBOOT_BOOTLOADER   \
  "Model01.rebootBootloader() is deprecated.\n"         \
  "Please use KeyboardHardware.resetDevice() instead."

namespace kaleidoscope {
namespace hardware {
namespace keyboardio {

class Model01 : public kaleidoscope::Hardware {
 public:
  Model01(void);

  static constexpr byte matrix_rows = 4;
  static constexpr byte matrix_columns = 16;
  static constexpr int8_t led_count = 64;

  typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;
  static constexpr int8_t numKeys() {
    return matrix_columns * matrix_rows;
  }

  void syncLeds(void);
  void setCrgbAt(KeyAddr key_addr, cRGB color);
  DEPRECATED(ROW_COL_FUNC) void setCrgbAt(byte row, byte col, cRGB color) {
    setCrgbAt(KeyAddr(row, col), color);
  }
  void setCrgbAt(int8_t i, cRGB crgb);
  cRGB getCrgbAt(int8_t i);
  int8_t getLedIndex(KeyAddr key_addr);
  DEPRECATED(ROW_COL_FUNC) int8_t getLedIndex(byte row, byte col) {
    return getLedIndex(KeyAddr(row, col));
  }

  void scanMatrix(void);
  void readMatrix(void);
  void actOnMatrixScan(void);
  void setup();
  void rebootBootloader() DEPRECATED(MODEL01_REBOOT_BOOTLOADER) {
    resetDevice();
  }
  void resetDevice() {
    bootloader_.resetDevice();
  }

  /* These public functions are things supported by the Model 01, but
   * aren't necessarily part of the Kaleidoscope API
   */
  void enableHighPowerLeds(void);
  void enableScannerPower(void);
  void setKeyscanInterval(uint8_t interval);
  boolean ledPowerFault(void);

  void maskKey(KeyAddr key_addr);
  DEPRECATED(ROW_COL_FUNC) void maskKey(byte row, byte col) {
    maskKey(KeyAddr(row, col));
  }
  void unMaskKey(KeyAddr key_addr);
  DEPRECATED(ROW_COL_FUNC) void unMaskKey(byte row, byte col) {
    unMaskKey(KeyAddr(row, col));
  }
  bool isKeyMasked(KeyAddr key_addr);
  DEPRECATED(ROW_COL_FUNC) bool isKeyMasked(byte row, byte col) {
    return isKeyMasked(KeyAddr(row, col));
  }
  void maskHeldKeys(void);

  bool isKeyswitchPressed(KeyAddr key_addr);
  DEPRECATED(ROW_COL_FUNC) bool isKeyswitchPressed(byte row, byte col) {
    return isKeyswitchPressed(KeyAddr(row, col));
  }
  bool isKeyswitchPressed(uint8_t keyIndex);
  uint8_t pressedKeyswitchCount();

  bool wasKeyswitchPressed(KeyAddr key_addr);
  DEPRECATED(ROW_COL_FUNC) bool wasKeyswitchPressed(byte row, byte col) {
    return wasKeyswitchPressed(KeyAddr(row, col));
  }
  bool wasKeyswitchPressed(uint8_t keyIndex);
  uint8_t previousPressedKeyswitchCount();

  void enableHardwareTestMode();

  keydata_t leftHandState;
  keydata_t rightHandState;
  keydata_t previousLeftHandState;
  keydata_t previousRightHandState;

 private:
  kaleidoscope::driver::bootloader::avr::Caterina bootloader_;
  static void actOnHalfRow(byte row, byte colState, byte colPrevState, byte startPos);

  static bool isLEDChanged;
  static KeyboardioScanner leftHand;
  static KeyboardioScanner rightHand;

  static keydata_t leftHandMask;
  static keydata_t rightHandMask;
};

}
}
}

#include "kaleidoscope/hardware/key_indexes.h"

#define PER_KEY_DATA_STACKED(dflt,                                    \
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
                                                                        \
    r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6, r0c7, r0c8, r0c9, r0c10, r0c11, r0c12, r0c13, r0c14, r0c15, \
    r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1c10, r1c11, r1c12, r1c13, r1c14, r1c15, \
    r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, r2c6, r2c7, r2c8, r2c9, r2c10, r2c11, r2c12, r2c13, r2c14, r2c15, \
    r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6, r3c7, r3c8, r3c9, r3c10, r3c11, r3c12, r3c13, r3c14, RESTRICT_ARGS_COUNT((r3c15), 64, KEYMAP_STACKED, ##__VA_ARGS__)

#define PER_KEY_DATA(dflt,                                                                                 \
  r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6,        r0c9,  r0c10, r0c11, r0c12, r0c13, r0c14, r0c15, \
  r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6,        r1c9,  r1c10, r1c11, r1c12, r1c13, r1c14, r1c15, \
  r2c0, r2c1, r2c2, r2c3, r2c4, r2c5,                     r2c10, r2c11, r2c12, r2c13, r2c14, r2c15, \
  r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r2c6,        r2c9,  r3c10, r3c11, r3c12, r3c13, r3c14, r3c15, \
              r0c7, r1c7, r2c7, r3c7,                             r3c8,  r2c8,  r1c8, r0c8,         \
                          r3c6,                                          r3c9, ...)                      \
    r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6, r0c7, r0c8, r0c9, r0c10, r0c11, r0c12, r0c13, r0c14, r0c15, \
    r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1c10, r1c11, r1c12, r1c13, r1c14, r1c15, \
    r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, r2c6, r2c7, r2c8, r2c9, r2c10, r2c11, r2c12, r2c13, r2c14, r2c15, \
    r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6, r3c7, r3c8, r3c9, r3c10, r3c11, r3c12, r3c13, r3c14, RESTRICT_ARGS_COUNT((r3c15), 64, KEYMAP, ##__VA_ARGS__)

#endif

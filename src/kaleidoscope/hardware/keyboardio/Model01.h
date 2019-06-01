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
#include "kaleidoscope/hardware/keyboardio/Model01_Spec.h"
#include "kaleidoscope/macro_helpers.h"

#define HARDWARE_IMPLEMENTATION kaleidoscope::hardware::keyboardio::Model01

#include "Kaleidoscope-HIDAdaptor-KeyboardioHID.h"
#include "KeyboardioScanner.h"
#include "kaleidoscope/Hardware.h"

namespace kaleidoscope {
namespace hardware {
namespace keyboardio {

class Model01 : public kaleidoscope::Hardware {
 public:
  Model01(void);

  static constexpr byte matrix_rows = Model01_Spec::matrix_rows;
  static constexpr byte matrix_columns = Model01_Spec::matrix_columns;
  static constexpr int8_t led_count = Model01_Spec::led_count;

  void syncLeds(void);
  void setCrgbAt(byte row, byte col, cRGB color);
  void setCrgbAt(int8_t i, cRGB crgb);
  cRGB getCrgbAt(int8_t i);
  int8_t getLedIndex(byte row, byte col) {
    return Model01_Spec::getLedIndex(row, col);
  }

  void scanMatrix(void);
  void readMatrix(void);
  void actOnMatrixScan(void);
  void setup();
  void rebootBootloader();

  /* These public functions are things supported by the Model 01, but
   * aren't necessarily part of the Kaleidoscope API
   */
  void enableHighPowerLeds(void);
  void enableScannerPower(void);
  void setKeyscanInterval(uint8_t interval);
  boolean ledPowerFault(void);

  void maskKey(byte row, byte col);
  void unMaskKey(byte row, byte col);
  bool isKeyMasked(byte row, byte col);
  void maskHeldKeys(void);

  bool isKeyswitchPressed(byte row, byte col);
  bool isKeyswitchPressed(uint8_t keyIndex);
  uint8_t pressedKeyswitchCount();

  bool wasKeyswitchPressed(byte row, byte col);
  bool wasKeyswitchPressed(uint8_t keyIndex);
  uint8_t previousPressedKeyswitchCount();

  void enableHardwareTestMode();

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
}

#include "kaleidoscope/hardware/key_indexes.h"

#endif

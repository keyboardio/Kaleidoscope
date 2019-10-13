/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Model01 -- Keyboard.io Model01 hardware support for Kaleidoscope
 * Copyright (C) 2017-2019  Keyboard.io, Inc
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

#include "KeyboardioScanner.h"

#include "kaleidoscope/driver/BaseKeyScanner.h"
#include "kaleidoscope/hardware/keyboardio/model01/Model01KeyScannerDescription.h"
#include "kaleidoscope/hardware/keyboardio/model01/Model01LedDriver.h"

namespace kaleidoscope {
namespace hardware {
namespace keyboardio {

class Model01;

class Model01KeyScanner : public kaleidoscope::driver::BaseKeyScanner<Model01KeyScannerDescription> {
  friend class Model01LedDriver;
  friend class Model01;
 public:
  static void setup();
  static void scanMatrix();
  static void readMatrix();
  static void actOnMatrixScan();

  static void maskKey(KeyAddr key_addr);
  static void unMaskKey(KeyAddr key_addr);
  static bool isKeyMasked(KeyAddr key_addr);
  static void maskHeldKeys();

  static bool isKeyswitchPressed(KeyAddr key_addr);
  static uint8_t pressedKeyswitchCount();

  static bool wasKeyswitchPressed(KeyAddr key_addr);
  static uint8_t previousPressedKeyswitchCount();

  static void setKeyscanInterval(uint8_t interval);

 protected:

  static keydata_t leftHandState;
  static keydata_t rightHandState;
  static keydata_t previousLeftHandState;
  static keydata_t previousRightHandState;

  static KeyboardioScanner leftHand;
  static KeyboardioScanner rightHand;

  static keydata_t leftHandMask;
  static keydata_t rightHandMask;

  static void actOnHalfRow(byte row, byte colState, byte colPrevState, byte startPos);
  static void enableScannerPower();
};

}
}
}

#endif

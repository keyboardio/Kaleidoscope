/* -*- mode: c++ -*-
 * ErgoDoxKeyScanner -- Combined ErgoDox KeyScanner for Kaleidoscope
 * Copyright (C) 2019  Keyboard.io, Inc
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

#include "kaleidoscope/driver/keyscanner/ATMegaKeyScanner.h"

#include "kaleidoscope/hardware/ez/ErgoDox/MCP23018KeyScanner.h"
#include "kaleidoscope/hardware/ez/ErgoDox/ErgoDoxKeyScannerDescription.h"

namespace kaleidoscope {
namespace hardware {
namespace ez {

template <typename _KeyScannerDescription>
class ErgoDoxKeyScanner : public kaleidoscope::driver::BaseKeyScanner<_KeyScannerDescription> {
 public:
  void setup() {
    atmega_scanner_.setup();
    expander_scanner_.setup();
  }
  void readMatrix() {
    atmega_scanner_.readMatrix();
    expander_scanner_.readMatrix();
  }
  void scanMatrix() {
    atmega_scanner_.scanMatrix();
    expander_scanner_.scanMatrix();
  }
  void actOnMatrixScan() {
    atmega_scanner_.actOnMatrixScan();
    expander_scanner_.actOnMatrixScan();
  }

  uint8_t pressedKeyswitchCount() {
    return atmega_scanner_.pressedKeyswitchCount() +
           expander_scanner_.pressedKeyswitchCount();
  }
  bool isKeyswitchPressed(KeyAddr key_addr) {
    if (key_addr.row() < 7) {
      return expander_scanner_.isKeyswitchPressed(key_addr);
    }
    KeyAddr new_addr = KeyAddr(key_addr.row() - 7, key_addr.col());
    return atmega_scanner_.isKeyswitchPressed(new_addr);
  }

  uint8_t previousPressedKeyswitchCount() {
    return atmega_scanner_.previousPressedKeyswitchCount() +
           expander_scanner_.previousPressedKeyswitchCount();
  }
  bool wasKeyswitchPressed(KeyAddr key_addr) {
    if (key_addr.row() < 7) {
      return expander_scanner_.wasKeyswitchPressed(key_addr);
    }
    KeyAddr new_addr = KeyAddr(key_addr.row() - 7, key_addr.col());
    return atmega_scanner_.wasKeyswitchPressed(new_addr);
  }

  void maskKey(KeyAddr key_addr) {
    if (key_addr.row() < 7) {
      return expander_scanner_.maskKey(key_addr);
    }
    KeyAddr new_addr = KeyAddr(key_addr.row() - 7, key_addr.col());
    return atmega_scanner_.maskKey(new_addr);
  }
  void unMaskKey(KeyAddr key_addr) {
    if (key_addr.row() < 7) {
      return expander_scanner_.unMaskKey(key_addr);
    }
    KeyAddr new_addr = KeyAddr(key_addr.row() - 7, key_addr.col());
    return atmega_scanner_.unMaskKey(new_addr);
  }
  bool isKeyMasked(KeyAddr key_addr) {
    if (key_addr.row() < 7) {
      return expander_scanner_.isKeyMasked(key_addr);
    }
    KeyAddr new_addr = KeyAddr(key_addr.row() - 7, key_addr.col());
    return atmega_scanner_.isKeyMasked(new_addr);
  }

  typedef typename kaleidoscope::driver::keyscanner::ATMegaKeyScanner<typename _KeyScannerDescription::ATMegaKeyScannerDescription> ATMegaKeyScanner;
  typedef typename _KeyScannerDescription::ATMegaKeyScannerDescription ATMegaKeyScannerDescription;

  typedef typename kaleidoscope::hardware::ez::ergodox::MCP23018KeyScanner<typename _KeyScannerDescription::ExpanderKeyScannerDescription> ExpanderKeyScanner;
  typedef typename _KeyScannerDescription::ExpanderKeyScannerDescription ExpanderKeyScannerDescription;

 private:
  ATMegaKeyScanner atmega_scanner_;
  ExpanderKeyScanner expander_scanner_;
};

}
}
}

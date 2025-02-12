/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2019-2025 Keyboard.io, inc.
 *
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * Additional Permissions:
 * As an additional permission under Section 7 of the GNU General Public
 * License Version 3, you may link this software against a Vendor-provided
 * Hardware Specific Software Module under the terms of the MCU Vendor
 * Firmware Library Additional Permission Version 1.0.
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

#include <stdint.h>  // for uint8_t

#include "kaleidoscope/MatrixAddr.h"  // IWYU pragma: keep
#include "kaleidoscope/key_defs.h"    // for Key

// IWYU pragma: no_include "kaleidoscope/KeyAddr.h"

namespace kaleidoscope {
namespace driver {
namespace keyscanner {

struct BaseProps {
  static constexpr uint8_t matrix_rows    = 0;
  static constexpr uint8_t matrix_columns = 0;
  typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;
};

template<typename _KeyScannerProps>
class Base {
 public:
  Base() {}

  typedef typename _KeyScannerProps::KeyAddr KeyAddr;

  static void handleKeyswitchEvent(Key mappedKey, KeyAddr key_addr, uint8_t keyState);

  void setup() {}
  void readMatrix() {}
  void scanMatrix() {}
  void actOnMatrixScan() {}

  uint8_t pressedKeyswitchCount() {
    return 0;
  }
  bool isKeyswitchPressed(KeyAddr key_addr) {
    return false;
  }

  uint8_t previousPressedKeyswitchCount() {
    return 0;
  }
  bool wasKeyswitchPressed(KeyAddr key_addr) {
    return false;
  }
};

}  // namespace keyscanner
}  // namespace driver
}  // namespace kaleidoscope

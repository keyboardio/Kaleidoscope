/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Alto -- Keyboardio Alto hardware support for Kaleidoscope
 * Copyright (C) 2021  Keyboard.io, Inc
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

#ifdef ARDUINO_keyboardio_alto

#include "kaleidoscope/driver/keyscanner/Base_Impl.h"  // For Base<>

#include "kaleidoscope/device/keyboardio/Alto.h"
#include "kaleidoscope/Runtime.h"


// Here, we set up aliases to the device's KeyScanner and KeyScannerProps
// in the global namespace within the scope of this file. We'll use these
// aliases to simplify some template initialization code below.
using KeyScannerProps = typename kaleidoscope::device::keyboardio::AltoProps::KeyScannerProps;
using KeyScanner      = typename kaleidoscope::device::keyboardio::AltoProps::KeyScanner;


namespace kaleidoscope {
namespace device {
namespace keyboardio {

// `KeyScannerProps` here refers to the alias set up above. We do not need to
// prefix the `matrix_rows` and `matrix_columns` names within the array
// declaration, because those are resolved within the context of the class, so
// the `matrix_rows` in `KeyScannerProps::matrix_row_pins[matrix_rows]` gets
// resolved as `KeyScannerProps::matrix_rows`.
const uint8_t KeyScannerProps::matrix_rows;
const uint8_t KeyScannerProps::matrix_columns;
constexpr uint8_t KeyScannerProps::matrix_row_pins[matrix_rows];
constexpr uint8_t KeyScannerProps::matrix_col_pins[matrix_columns];
//
//// `KeyScanner` here refers to the alias set up above, just like in the
//// `KeyScannerProps` case above.
template<>
KeyScanner::row_state_t KeyScanner::matrix_state_[KeyScannerProps::matrix_rows] = {};

template<>
uint32_t KeyScanner::next_scan_at_ = 0;


#ifndef KALEIDOSCOPE_VIRTUAL_BUILD

/********* Hardware plugin *********/

void Alto::rebootBootloader() {
  USBCore().disconnect();
  NVIC_SystemReset();
}

#endif

}  // namespace keyboardio
}  // namespace device
}  // namespace kaleidoscope

#endif

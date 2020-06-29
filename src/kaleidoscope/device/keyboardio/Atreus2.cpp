/* -*- mode: c++ -*-
 * Keyboardio Atreus hardware support for Kaleidoscope
 * Copyright (C) 2019  Keyboard.io, Inc
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

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
#ifdef ARDUINO_AVR_KEYBOARDIO_ATREUS

#include "kaleidoscope/Runtime.h"
#include "kaleidoscope/driver/keyscanner/Base_Impl.h"

using namespace kaleidoscope::device::keyboardio;

using KeyScannerProps = typename AtreusProps::KeyScannerProps;
using KeyScanner = typename AtreusProps::KeyScanner;

namespace kaleidoscope {
namespace device {
namespace keyboardio {

const uint8_t KeyScannerProps::matrix_rows;
const uint8_t KeyScannerProps::matrix_columns;

constexpr uint8_t KeyScannerProps::matrix_row_pins[matrix_rows];
constexpr uint8_t KeyScannerProps::matrix_col_pins[matrix_columns];

template<> uint16_t KeyScanner::previousKeyState_[KeyScannerProps::matrix_rows] = {};
template<> uint16_t KeyScanner::keyState_[KeyScannerProps::matrix_rows] = {};
template<> uint16_t KeyScanner::masks_[KeyScannerProps::matrix_rows] = {};
template<> uint8_t KeyScanner::debounce_matrix_[KeyScannerProps::matrix_rows][KeyScannerProps::matrix_columns] = {};

ISR(TIMER1_OVF_vect) {
  Runtime.device().keyScanner().do_scan_ = true;
}

}
}
}

#endif
#endif // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

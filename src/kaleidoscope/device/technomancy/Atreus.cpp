/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Technomancy-Atreus -- Atreus hardware support for Kaleidoscope
 * Copyright (C) 2018, 2019  Keyboard.io, Inc
 *
 * Based on QMK (commit e9a67f8fd)
 *  (C) Jack Humbert, Phil Hagelberg, and others
 * Original QMK sources:
 *  - keyboards/atreus/atreus.h
 *  - keyboards/atreus/config.h
 *  - quantum/matrix.c
 *  - tmk_core/common/avr/bootloader.c
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
#ifdef ARDUINO_AVR_ATREUS

#include "kaleidoscope/Runtime.h"
#include "kaleidoscope/driver/keyscanner/Base_Impl.h"

// Here, we set up aliases to the device's KeyScanner and KeyScannerProps 
// in the global namespace within the scope of this file. We'll use these
// aliases to simplify some template initialization code below.
using KeyScannerProps = typename kaleidoscope::device::technomancy::AtreusProps::KeyScannerProps;
using KeyScanner = typename kaleidoscope::device::technomancy::AtreusProps::KeyScanner;


namespace kaleidoscope {
namespace device {
namespace technomancy {

// `KeyScannerProps` here refers to the alias set up above. We do not need to
// prefix the `matrix_rows` and `matrix_columns` names within the array
// declaration, because those are resolved within the context of the class, so
// the `matrix_rows` in `KeyScannerProps::matrix_row_pins[matrix_rows]` gets
// resolved as `KeyScannerProps::matrix_rows`.
const uint8_t KeyScannerProps::matrix_rows;
const uint8_t KeyScannerProps::matrix_columns;
constexpr uint8_t KeyScannerProps::matrix_row_pins[matrix_rows];
constexpr uint8_t KeyScannerProps::matrix_col_pins[matrix_columns];

// Resolving is a bit different in case of templates, however: the name of the
// array is resolved within the scope of the namespace and the class, but the
// array size is not - because it is a template. Therefore, we need a fully
// qualified name there - or an alias in the global scope, which we set up just
// above.
template<> uint16_t KeyScanner::previousKeyState_[KeyScannerProps::matrix_rows] = {};
template<> uint16_t KeyScanner::keyState_[KeyScannerProps::matrix_rows] = {};
template<> uint16_t KeyScanner::masks_[KeyScannerProps::matrix_rows] = {};
template<> uint8_t KeyScanner::debounce_matrix_[KeyScannerProps::matrix_rows][KeyScannerProps::matrix_columns] = {};

// We set up the TIMER1 interrupt vector here. Due to dependency reasons, this
// cannot be in a header-only driver, and must be placed here.
//
// Timer1 is responsible for setting a property on the KeyScanner, which will
// tell it to do a scan. We use this to make sure that scans happen at roughly
// the intervals we want. We do the scan outside of the interrupt scope for
// practical reasons: guarding every codepath against interrupts that can be
// reached from the scan is far too tedious, for very little gain.
ISR(TIMER1_OVF_vect) {
  Runtime.device().keyScanner().do_scan_ = true;
}

} // namespace technomancy
} // namespace device
} // namespace kaleidoscope

#endif
#endif // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

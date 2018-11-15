/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-OLKB-Planck -- Planck hardware support for Kaleidoscope
 * Copyright (C) 2018  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef ARDUINO_AVR_PLANCK

#include <Kaleidoscope.h>
#include <avr/wdt.h>

namespace kaleidoscope {
namespace hardware {
namespace olkb {

const int8_t Planck::matrix_rows;
const int8_t Planck::matrix_columns;
constexpr uint8_t Planck::matrix_row_pins[matrix_rows];
constexpr uint8_t Planck::matrix_col_pins[matrix_columns];
constexpr int8_t Planck::led_count;
uint16_t Planck::previousKeyState_[matrix_rows];
uint16_t Planck::keyState_[matrix_rows];
uint16_t Planck::masks_[matrix_rows];
uint8_t Planck::debounce_matrix_[matrix_rows][matrix_columns];

ISR(TIMER1_OVF_vect) {
  Planck::do_scan_ = true;
}
}
}
}

HARDWARE_IMPLEMENTATION KeyboardHardware;
kaleidoscope::hardware::olkb::Planck &Planck = KeyboardHardware;

#endif

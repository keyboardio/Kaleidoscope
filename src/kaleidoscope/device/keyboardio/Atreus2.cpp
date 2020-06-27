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

namespace kaleidoscope {
namespace device {
namespace keyboardio {
  const uint8_t kaleidoscope::Device::KeyScannerProps::matrix_rows;                 
  const uint8_t kaleidoscope::Device::KeyScannerProps::matrix_columns; 
  constexpr uint8_t kaleidoscope::Device::KeyScannerProps::matrix_row_pins[matrix_rows];                
  constexpr uint8_t kaleidoscope::Device::KeyScannerProps::matrix_col_pins[matrix_columns];             
  template<> uint16_t kaleidoscope::Device::KeyScanner::previousKeyState_[kaleidoscope::Device::KeyScannerProps::matrix_rows] = {}; 
  template<> uint16_t kaleidoscope::Device::KeyScanner::keyState_[kaleidoscope::Device::KeyScannerProps::matrix_rows] = {};         
  template<> uint16_t kaleidoscope::Device::KeyScanner::masks_[kaleidoscope::Device::KeyScannerProps::matrix_rows] = {};            
  template<> uint8_t kaleidoscope::Device::KeyScanner::debounce_matrix_[kaleidoscope::Device::KeyScannerProps::matrix_rows][kaleidoscope::Device::KeyScannerProps::matrix_columns] = {}; 
                                                                                           
  ISR(TIMER1_OVF_vect) {                                                                   
    Runtime.device().keyScanner().do_scan_ = true;                                         
  }

}
}
}

#endif
#endif // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Spacecadet -- Keyboardio Spacecadet hardware support for Kaleidoscope
 * Copyright (C) 2017-2021  Keyboard.io, Inc
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

#ifdef ARDUINO_keyboardio_spacecadet

#ifndef EEPROM_EMULATION_SIZE
#define EEPROM_EMULATION_SIZE 4096
#endif

#include <Arduino.h>


#include "kaleidoscope/driver/keyscanner/Base.h"
#include "kaleidoscope/driver/storage/GD32Flash.h"
#include "kaleidoscope/device/Base.h"
#include "kaleidoscope/driver/hid/Keyboardio.h"
#include "kaleidoscope/driver/bootloader/gd32/Base.h"
#include "kaleidoscope/device/keyboardio/GD32Keyscanner.h"

namespace kaleidoscope {
namespace device {
namespace keyboardio {

struct SpacecadetStorageProps: public kaleidoscope::driver::storage::GD32FlashProps {
  static constexpr uint16_t length = EEPROM_EMULATION_SIZE;
};



struct SpacecadetProps : public kaleidoscope::device::BaseProps {
  typedef kaleidoscope::driver::hid::KeyboardioProps HIDProps;
  typedef kaleidoscope::driver::hid::Keyboardio<HIDProps> HID;

  struct KeyScannerProps : public kaleidoscope::driver::keyscanner::GD32Props {


    static constexpr uint8_t matrix_rows = 6;
    static constexpr uint8_t matrix_columns = 21;
    typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;
    static constexpr uint8_t matrix_row_pins[matrix_rows] =  {PB15, PB14, PB13, PB12, PB11, PB10};
    static constexpr uint8_t matrix_col_pins[matrix_columns] = {PB1, PB0,PA7,PA6,PA5,PA4,PA3,PA2,PA1,PA0,PD1,PD0,PC15,PC14,PC13, PA9,PA10, PB8, PB7, PB6,PB5};

  };

  typedef kaleidoscope::driver::keyscanner::GD32<KeyScannerProps> KeyScanner;


  typedef SpacecadetStorageProps StorageProps;
  typedef kaleidoscope::driver::storage::GD32Flash<StorageProps> Storage;

  typedef kaleidoscope::driver::bootloader::gd32::Base BootLoader;
  static constexpr const char *short_name = "spacecadet";
};


class Spacecadet : public kaleidoscope::device::Base<SpacecadetProps> {
 public:
};


} // namespace keyboardio
} // namespace device

EXPORT_DEVICE(kaleidoscope::device::keyboardio::Spacecadet)

}


#define PER_KEY_DATA(dflt,                                                                                 \
     r0c0,       r0c2, r0c3,       r0c5,       r0c7,       r0c9,        r0c11,        r0c13,        r0c15,        r0c17, r0c18, r0c19, \
     r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1c10, r1c11, r1c12, r1c13, r1c14, r1c15, r1c16, r1c17, r1c18, r1c19, r1c20, \
     r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, r2c6, r2c7, r2c8, r2c9, r2c10, r2c11, r2c12, r2c13, r2c14, r2c15, r2c16, r2c17, r2c18, r2c19, \
     r3c0,       r3c2, r3c3, r3c4, r3c5, r3c6, r3c7, r3c8, r3c9, r3c10, r3c11, r3c12, r3c13, r3c14, r3c15,        r3c17, r3c18, r3c20, \
     r4c0, r4c1, r4c2, r4c3,       r4c5, r4c6, r4c7, r4c8, r4c9, r4c10, r4c11, r4c12, r4c13, r4c14, r4c15,        r4c17, r4c18, r4c19, r4c20, \
     r5c0, r5c1, r5c2, r5c3,                               r5c9,                                    r5c15,        r5c17, r5c18, r5c19, ...) \
\
\
     r0c0, XXX,  r0c2, r0c3, XXX,  r0c5, XXX,  r0c7, XXX,  r0c9, XXX,   r0c11, XXX,   r0c13, XXX,   r0c15, XXX,   r0c17, r0c18, r0c19, XXX,   \
     r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1c10, r1c11, r1c12, r1c13, r1c14, r1c15, r1c16, r1c17, r1c18, r1c19, r1c20, \
     r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, r2c6, r2c7, r2c8, r2c9, r2c10, r2c11, r2c12, r2c13, r2c14, r2c15, r2c16, r2c17, r2c18, r2c19, XXX,   \
     r3c0, XXX,  r3c2, r3c3, r3c4, r3c5, r3c6, r3c7, r3c8, r3c9, r3c10, r3c11, r3c12, r3c13, r3c14, r3c15, XXX,   r3c17, r3c18, XXX,   r3c20, \
     r4c0, r4c1, r4c2, r4c3, XXX,  r4c5, r4c6, r4c7, r4c8, r4c9, r4c10, r4c11, r4c12, r4c13, r4c14, r4c15, XXX,   r4c17, r4c18, r4c19, r4c20, \
     r5c0, r5c1, r5c2, r5c3, XXX,  XXX,  XXX,  XXX,  XXX,  r5c9, XXX,   XXX,   XXX,   XXX,   XXX,   XXX,   r5c15, XXX,   r5c17, r5c18, r5c19

#endif

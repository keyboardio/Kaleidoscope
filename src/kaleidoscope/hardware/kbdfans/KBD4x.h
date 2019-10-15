/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-KBDFans-KBD4x -- KBD4x hardware support for Kaleidoscope
 * Copyright (C) 2019  Keyboard.io, Inc
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

#pragma once

#ifdef ARDUINO_AVR_KBD4X

#define KALEIDOSCOPE_BOOTLOADER_FLIP 1

#include <Arduino.h>
#define HARDWARE_IMPLEMENTATION kaleidoscope::hardware::kbdfans::KBD4x

#include "kaleidoscope/hardware/kbdfans/kbd4x/KBD4xKeyScannerDescription.h"

#include "kaleidoscope/driver/keyscanner/ATMegaKeyScanner.h"
#include "kaleidoscope/driver/bootloader/avr/FLIP.h"
#include "kaleidoscope/hardware/avr/AVRDeviceDescription.h"

#include "kaleidoscope/Device.h"

namespace kaleidoscope {
namespace hardware {
namespace kbdfans {

struct KBD4xDeviceDescription : kaleidoscope::hardware::avr::AVRDeviceDescription {
  typedef KBD4xKeyScannerDescription KeyScannerDescription;
  typedef kaleidoscope::driver::keyscanner::ATMegaKeyScanner<KBD4xKeyScannerDescription> KeyScanner;
  typedef kaleidoscope::driver::bootloader::avr::FLIP BootLoader;
};

class KBD4x: public kaleidoscope::Device<KBD4xDeviceDescription> {
 public:
  KBD4x(void) {
    mcu_.disableJTAG();
    mcu_.disableClockDivision();
  }
};

#define PER_KEY_DATA(dflt,                                                       \
         R0C0, R0C1, R0C2, R0C3, R0C4, R0C5, R0C6, R0C7, R0C8, R0C9, R0C10, R0C11, \
         R1C0, R1C1, R1C2, R1C3, R1C4, R1C5, R1C6, R1C7, R1C8, R1C9, R1C10, R1C11, \
         R2C0, R2C1, R2C2, R2C3, R2C4, R2C5, R2C6, R2C7, R2C8, R2C9, R2C10, R2C11, \
         R3C0, R3C1, R3C2, R3C3, R3C4,    R3C5,    R3C7, R3C8, R3C9, R3C10, R3C11  \
  )                                                                                \
         R0C0, R0C1, R0C2, R0C3, R0C4, R0C5, R0C6, R0C7, R0C8, R0C9, R0C10, R0C11, \
         R1C0, R1C1, R1C2, R1C3, R1C4, R1C5, R1C6, R1C7, R1C8, R1C9, R1C10, R1C11, \
         R2C0, R2C1, R2C2, R2C3, R2C4, R2C5, R2C6, R2C7, R2C8, R2C9, R2C10, R2C11, \
         R3C0, R3C1, R3C2, R3C3, R3C4, R3C5, R3C5, R3C7, R3C8, R3C9, R3C10, R3C11
}

}
}

#include "kaleidoscope/hardware/key_indexes.h"

extern kaleidoscope::hardware::kbdfans::KBD4x &KBD4x;

#endif

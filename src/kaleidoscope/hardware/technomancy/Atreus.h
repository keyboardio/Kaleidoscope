/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Technomancy-Atreus -- Atreus hardware support for Kaleidoscope
 * Copyright (C) 2018, 2019  Keyboard.io, Inc
 *
 * Based on QMK (commit e9a67f8fd)
 *  (C) Jack Humbert, Jun Wako, Phil Hagelberg, and others
 * Original QMK sources:
 *  - keyboards/atreus/atreus.h
 *  - keyboards/atreus/config.h
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

#pragma once

#ifdef ARDUINO_AVR_ATREUS

#include <Arduino.h>
#define HARDWARE_IMPLEMENTATION kaleidoscope::hardware::technomancy::Atreus

#include "kaleidoscope/hardware/technomancy/atreus/AtreusKeyScannerDescription.h"
#include "kaleidoscope/driver/keyscanner/ATMegaKeyScanner.h"
#include "kaleidoscope/driver/mcu/ATMega32U4.h"
#include "kaleidoscope/driver/bootloader/avr/HalfKay.h"
#include "kaleidoscope/driver/storage/AVREEPROM.h"

#include "kaleidoscope/DeviceDescription.h"
#include "kaleidoscope/Device.h"

namespace kaleidoscope {
namespace hardware {
namespace technomancy {

struct AtreusDeviceDescription : kaleidoscope::DeviceDescription {
  typedef AtreusKeyScannerDescription KeyScannerDescription;
  typedef kaleidoscope::driver::keyscanner::ATMegaKeyScanner<AtreusKeyScannerDescription> KeyScanner;
  typedef kaleidoscope::driver::mcu::ATMega32U4 MCU;
  typedef kaleidoscope::driver::bootloader::avr::HalfKay BootLoader;
  typedef kaleidoscope::driver::storage::AVREEPROMStorageDescription StorageDescription;
  typedef kaleidoscope::driver::storage::AVREEPROM<StorageDescription> Storage;
};

class Atreus: public kaleidoscope::Device<AtreusDeviceDescription> {
 public:
  Atreus() {}
};

#define PER_KEY_DATA(dflt,                                                  \
    R0C0, R0C1, R0C2, R0C3, R0C4,             R0C7, R0C8, R0C9, R0C10, R0C11, \
    R1C0, R1C1, R1C2, R1C3, R1C4,             R1C7, R1C8, R1C9, R1C10, R1C11, \
    R2C0, R2C1, R2C2, R2C3, R2C4,             R2C7, R2C8, R2C9, R2C10, R2C11, \
    R3C0, R3C1, R3C2, R3C3, R3C4, R3C5, R3C6, R3C7, R3C8, R3C9, R3C10, R3C11  \
  )                                                                           \
                                                                              \
    R0C0, R0C1, R0C2, R0C3, R0C4, XXX,    R0C7, R0C8, R0C9, R0C10, R0C11,     \
    R1C0, R1C1, R1C2, R1C3, R1C4, XXX,    R1C7, R1C8, R1C9, R1C10, R1C11,     \
    R2C0, R2C1, R2C2, R2C3, R2C4, R3C5,   R2C7, R2C8, R2C9, R2C10, R2C11,     \
    R3C0, R3C1, R3C2, R3C3, R3C4, R3C6,   R3C7, R3C8, R3C9, R3C10, R3C11

#define PER_KEY_DATA_STACKED(dflt,                                          \
    R0C0, R0C1, R0C2, R0C3, R0C4,                                             \
    R1C0, R1C1, R1C2, R1C3, R1C4,                                             \
    R2C0, R2C1, R2C2, R2C3, R2C4,                                             \
    R3C0, R3C1, R3C2, R3C3, R3C4, R3C5,                                       \
                                                                              \
          R0C7, R0C8, R0C9, R0C10, R0C11,                                     \
          R1C7, R1C8, R1C9, R1C10, R1C11,                                     \
          R2C7, R2C8, R2C9, R2C10, R2C11,                                     \
    R3C6, R3C7, R3C8, R3C9, R3C10, R3C11                                      \
  )                                                                           \
    R0C0, R0C1, R0C2, R0C3, R0C4, dflt,   R0C7, R0C8, R0C9, R0C10, R0C11,     \
    R1C0, R1C1, R1C2, R1C3, R1C4, dflt,   R1C7, R1C8, R1C9, R1C10, R1C11,     \
    R2C0, R2C1, R2C2, R2C3, R2C4, R3C5,   R2C7, R2C8, R2C9, R2C10, R2C11,     \
    R3C0, R3C1, R3C2, R3C3, R3C4, R3C6,   R3C7, R3C8, R3C9, R3C10, R3C11
}
}
}

#include "kaleidoscope/hardware/key_indexes.h"

extern kaleidoscope::hardware::technomancy::Atreus &Atreus;

#endif

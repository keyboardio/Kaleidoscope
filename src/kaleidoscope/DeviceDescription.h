/* -*- mode: c++ -*-
 * DeviceDescription -- Device description base class
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

#include "kaleidoscope/driver/keyscanner/NoOpKeyScanner.h"
#include "kaleidoscope/driver/led/NoLedsDescription.h"
#include "kaleidoscope/driver/led/NoLeds.h"
#include "kaleidoscope/driver/mcu/NoMCU.h"
#include "kaleidoscope/driver/bootloader/NoBootloader.h"
#include "kaleidoscope/driver/storage/NoStorageDescription.h"
#include "kaleidoscope/driver/storage/NoStorage.h"

namespace kaleidoscope {

struct DeviceDescription {
  typedef kaleidoscope::driver::keyscanner::NoOpKeyScannerDescription KeyScannerDescription;
  typedef kaleidoscope::driver::keyscanner::NoOpKeyScanner<KeyScannerDescription> KeyScanner;
  typedef kaleidoscope::driver::led::NoLedsDescription LEDsDescription;
  typedef kaleidoscope::driver::led::NoLeds<LEDsDescription> LEDs;
  typedef kaleidoscope::driver::mcu::NoMCU MCU;
  typedef kaleidoscope::driver::bootloader::NoBootloader BootLoader;
  typedef kaleidoscope::driver::storage::NoStorageDescription StorageDescription;
  typedef kaleidoscope::driver::storage::NoStorage<StorageDescription> Storage;
};

}

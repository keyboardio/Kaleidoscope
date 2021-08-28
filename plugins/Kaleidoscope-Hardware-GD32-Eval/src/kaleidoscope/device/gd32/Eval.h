/* -*- mode: c++ -*-
 * Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2021  Keyboard.io, Inc.
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

#ifdef ARDUINO_GD32F303ZE_EVAL

#include <Arduino.h>

#include "kaleidoscope/device/Base.h"
#include "kaleidoscope/driver/storage/GD32Flash.h"
#include "kaleidoscope/driver/bootloader/gd32/Base.h"

#include "kaleidoscope/device/gd32/eval/KeyScanner.h"

namespace kaleidoscope {
namespace device {
namespace gd32 {

struct EvalStorageProps: kaleidoscope::driver::storage::GD32FlashProps {};

struct EvalProps: kaleidoscope::device::BaseProps {
  typedef kaleidoscope::driver::bootloader::gd32::Base BootLoader;
  typedef eval::KeyScannerProps KeyScannerProps;
  typedef eval::KeyScanner KeyScanner;
  typedef EvalStorageProps StorageProps;
  typedef kaleidoscope::driver::storage::GD32Flash<StorageProps> Storage;
  static constexpr const char *short_name = "GD32Eval";
};

class Eval: public kaleidoscope::device::Base<EvalProps> {};

#define PER_KEY_DATA(dflt,                                           \
         R0C0, R0C1                                                  \
  )                                                                  \
         R0C0, R0C1

} // namespace gd32
} // namespace device

EXPORT_DEVICE(kaleidoscope::device::gd32::Eval)

} // namespace kaleidoscope

#endif

/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Model01 -- Keyboard.io Model01 hardware support for Kaleidoscope
 * Copyright (C) 2017-2019  Keyboard.io, Inc
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

// IWYU pragma: private, include "kaleidoscope/device/device.h"

#pragma once

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD

// Compile-time (emulated) hardware header:
#include KALEIDOSCOPE_HARDWARE_H

// From system:
#include <stdint.h>  // for uint8_t
// From Arduino libraries:
#include <HardwareSerial.h>  // for Serial
// From Kaleidoscope:
#include "kaleidoscope/device/Base.h"             // for Base
#include "kaleidoscope/driver/bootloader/None.h"  // for None
#include "kaleidoscope/driver/hid/Keyboardio.h"   // for Keyboardio
#include "kaleidoscope/driver/keyscanner/Base.h"  // for Base
#include "kaleidoscope/driver/mcu/None.h"         // for None

namespace kaleidoscope {
namespace device {
namespace virt {

class VirtualKeyScanner
  : public kaleidoscope::driver::keyscanner::Base<kaleidoscope::DeviceProps::KeyScannerProps> {
 private:
  typedef VirtualKeyScanner ThisType;
  typedef kaleidoscope::driver::keyscanner::Base<kaleidoscope::DeviceProps::KeyScannerProps> ParentType;

 public:
  typedef typename ParentType::KeyAddr KeyAddr;

  enum class KeyState {
    Pressed,
    NotPressed,
    Tap
  };

  static constexpr uint8_t matrix_rows    = kaleidoscope::DeviceProps::KeyScannerProps::matrix_rows;
  static constexpr uint8_t matrix_columns = kaleidoscope::DeviceProps::KeyScannerProps::matrix_columns;

  VirtualKeyScanner();

  void setup();
  void readMatrix();
  void scanMatrix() {
    this->readMatrix();
    this->actOnMatrixScan();
  }
  void actOnMatrixScan();

  uint8_t pressedKeyswitchCount() const;
  bool isKeyswitchPressed(KeyAddr key_addr) const;

  uint8_t previousPressedKeyswitchCount() const;
  bool wasKeyswitchPressed(KeyAddr key_addr) const;

  void setEnableReadMatrix(bool state) {
    read_matrix_enabled_ = state;
  }

  void setKeystate(KeyAddr keyAddr, KeyState ks);
  KeyState getKeystate(KeyAddr keyAddr) const;

 private:
  bool anythingHeld();

 private:
  uint8_t n_pressed_switches_,
    n_previously_pressed_switches_;

  bool read_matrix_enabled_;

  KeyState keystates_[matrix_rows * matrix_columns];       // NOLINT(runtime/arrays)
  KeyState keystates_prev_[matrix_rows * matrix_columns];  // NOLINT(runtime/arrays)
};

class VirtualLEDDriver
  : public driver::led::Base<kaleidoscope::DeviceProps::LEDDriverProps> {
 public:
  typedef driver::led::Base<kaleidoscope::DeviceProps::LEDDriverProps>
    ParentType;

  static constexpr uint8_t led_count = kaleidoscope::DeviceProps::LEDDriverProps::led_count;

  void setup();
  void syncLeds();
  void setCrgbAt(uint8_t i, cRGB color);
  cRGB getCrgbAt(uint8_t i) const;

 private:
  cRGB led_states_[led_count];  // NOLINT(runtime/arrays)
};

// This overrides only the drivers and keeps the driver props of
// the physical keyboard.
//
struct VirtualProps : public kaleidoscope::DeviceProps {
  typedef kaleidoscope::driver::hid::KeyboardioProps HIDProps;
  typedef kaleidoscope::driver::hid::Keyboardio<HIDProps> HID;
  typedef typename kaleidoscope::DeviceProps::KeyScannerProps
    KeyScannerProps;
  typedef VirtualKeyScanner
    KeyScanner;
  typedef KeyScannerProps::KeyAddr
    KeyAddr;

  typedef typename kaleidoscope::DeviceProps::LEDDriverProps
    LEDDriverProps;
  typedef VirtualLEDDriver
    LEDDriver;

  typedef kaleidoscope::driver::mcu::None MCU;

  typedef kaleidoscope::driver::bootloader::None
    Bootloader;

  typedef typename kaleidoscope::DeviceProps::StorageProps
    StorageProps;
  typedef typename kaleidoscope::DeviceProps::Storage
    Storage;
};

class VirtualDevice
  : public kaleidoscope::device::Base<device::virt::VirtualProps> {
 public:
  auto serialPort() -> decltype(Serial) & {
    return Serial;
  }
};

}  // namespace virt
}  // namespace device

typedef device::virt::VirtualDevice Device;

}  // namespace kaleidoscope

#endif  // ifdef KALEIDOSCOPE_VIRTUAL_BUILD

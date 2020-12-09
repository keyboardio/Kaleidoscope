/* -*- mode: c++ -*-
 * device::ATmega32U4Keyboard -- Generic ATmega32U4 keyboard base class
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

#if defined(__SAMD51__) || defined(__SAMD21__)  || defined(KALEIDOSCOPE_VIRTUAL_BUILD)

#include <Arduino.h>
#include "kaleidoscope/device/Base.h"

#include "kaleidoscope/driver/mcu/SAMD.h"
#include "kaleidoscope/driver/keyscanner/SAMD.h"
#include "kaleidoscope/driver/storage/Flash.h"

namespace kaleidoscope {
namespace device {

struct SAMDStorageProps : public kaleidoscope::driver::storage::FlashProps {
  static constexpr uint16_t length = EEPROM_EMULATION_SIZE;
};

struct SAMDKeyboardProps : kaleidoscope::device::BaseProps {
  typedef kaleidoscope::driver::mcu::SAMD MCU;
  typedef SAMDStorageProps StorageProps;
  typedef kaleidoscope::driver::storage::Flash<StorageProps> Storage;
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
template <typename _DeviceProps>
class SAMDKeyboard : public kaleidoscope::device::Base<_DeviceProps> {
 public:
  auto serialPort() -> decltype(Serial) & {
    return Serial;
  }
};
#else // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
template <typename _DeviceProps>
class SAMDKeyboard;
#endif // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

}
}

#endif

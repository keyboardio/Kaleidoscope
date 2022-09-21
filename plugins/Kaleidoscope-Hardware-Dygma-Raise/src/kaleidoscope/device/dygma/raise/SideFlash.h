/* -*- mode: c++ -*-
 * kaleidoscope::device::dygma::Raise -- Kaleidoscope device plugin for Dygma Raise
 * Copyright (C) 2017-2019  Keyboard.io, Inc
 * Copyright (C) 2017-2019  Dygma Lab S.L.
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

#ifdef ARDUINO_SAMD_RAISE

#include "kaleidoscope/Runtime.h"
#include "kaleidoscope/plugin.h"

namespace kaleidoscope {
namespace device {
namespace dygma {
namespace raise {

template<typename _Firmware>
class SideFlash : public kaleidoscope::Plugin {
 private:
  _Firmware firmware;

 public:
  EventHandlerResult onFocusEvent(const char *input) {
    const char *cmd_flash_left   = PSTR("hardware.flash_left_side");
    const char *cmd_flash_right  = PSTR("hardware.flash_right_side");
    const char *cmd_verify_left  = PSTR("hardware.verify_left_side");
    const char *cmd_verify_right = PSTR("hardware.verify_right_side");

    if (::Focus.inputMatchesHelp(input))
      return ::Focus.printHelp(cmd_flash_left, cmd_flash_right, cmd_verify_left, cmd_verify_right);

    auto sideFlasher           = Runtime.device().sideFlasher();
    uint8_t left_boot_address  = Runtime.device().side.left_boot_address;
    uint8_t right_boot_address = Runtime.device().side.right_boot_address;
    enum {
      FLASH,
      VERIFY
    } sub_command;
    uint8_t address = 0;

    if (::Focus.inputMatchesCommand(input, cmd_flash_left)) {
      sub_command = FLASH;
      address     = left_boot_address;
    } else if (::Focus.inputMatchesCommand(input, cmd_flash_right)) {
      sub_command = FLASH;
      address     = right_boot_address;
    } else if (::Focus.inputMatchesCommand(input, cmd_verify_left)) {
      sub_command = VERIFY;
      address     = left_boot_address;
    } else if (::Focus.inputMatchesCommand(input, cmd_verify_right)) {
      sub_command = VERIFY;
      address     = right_boot_address;
    } else {
      return EventHandlerResult::OK;
    }

    bool result;
    Runtime.device().side.prepareForFlash();
    if (sub_command == FLASH)
      result = sideFlasher.flash(address, firmware);
    else
      result = sideFlasher.verify(address, firmware);
    ::Focus.send(result);

    return EventHandlerResult::EVENT_CONSUMED;
  }
};

}  // namespace raise
}  // namespace dygma
}  // namespace device
}  // namespace kaleidoscope

#endif

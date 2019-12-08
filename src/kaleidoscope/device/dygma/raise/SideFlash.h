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

template <typename _Firmware>
class SideFlash : public kaleidoscope::Plugin {
 private:
  _Firmware firmware;
 public:
  EventHandlerResult onFocusEvent(const char *command) {
    if (::Focus.handleHelp(command, PSTR("hardware.flash_left_side\nhardware.flash_right_side\nhardware.verify_left_side\nhardware.verify_right_side")))
      return EventHandlerResult::OK;

    if (strncmp_P(command, PSTR("hardware."), 9) != 0)
      return EventHandlerResult::OK;

    auto sideFlasher = Runtime.device().sideFlasher();
    uint8_t left_boot_address = Runtime.device().side.left_boot_address;
    uint8_t right_boot_address = Runtime.device().side.right_boot_address;
    enum {
      FLASH,
      VERIFY
    } sub_command;
    uint8_t address = 0;

    if (strcmp_P(command + 9, PSTR("flash_left_side")) == 0) {
      sub_command = FLASH;
      address = left_boot_address;
    } else if (strcmp_P(command + 9, PSTR("flash_right_side")) == 0) {
      sub_command = FLASH;
      address = right_boot_address;
    } else if (strcmp_P(command + 9, PSTR("verify_left_side")) == 0) {
      sub_command = VERIFY;
      address = left_boot_address;
    } else if (strcmp_P(command + 9, PSTR("verify_right_side")) == 0) {
      sub_command = VERIFY;
      address = right_boot_address;
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

}
}
}
}

#endif

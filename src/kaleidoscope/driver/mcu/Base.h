/* -*- mode: c++ -*-
 * driver::mcu::Base -- MCU driver base class for Kaleidoscope
 * Copyright (C) 2019, 2020  Keyboard.io, Inc
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

namespace kaleidoscope {
namespace driver {
namespace mcu {

struct BaseProps {};

template<typename _Props>
class Base {
 public:
  Base() {}

  void setup() {}

  /**
   * Detach the device from the host.
   *
   * Must detach the device, without rebooting or cutting power. Only the end
   * points should get detached, the device must remain powered on.
   */
  void detachFromHost() {}
  /**
   * Attack the device to the host.
   *
   * Must restore the link detachFromHost severed.
   */
  void attachToHost() {}

  virtual bool USBConfigured() {
    return true;
  }

  /**
   * Poll the USB device for a bus reset.
   *
   * This default implementation uses a change in USBConfigured() as a proxy
   * for actually detecting a bus reset.
   */
  bool pollUSBReset() {
    static bool was_configured;
    if (was_configured) {
      if (!USBConfigured()) {
        was_configured = false;
        return true;
      }
    } else {
      if (USBConfigured()) {
        was_configured = true;
      }
    }
    return false;
  }
};

}  // namespace mcu
}  // namespace driver
}  // namespace kaleidoscope

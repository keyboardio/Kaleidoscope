/* -*- mode: c++ -*-
 * kaleidoscope::driver::bootloader::avr::Caterina -- Driver for the Caterina bootloader
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

#include <avr/wdt.h>

namespace kaleidoscope {
namespace driver {
namespace bootloader {
namespace avr {

class Caterina {
 public:
  Caterina() {}

  static void resetDevice() {
    // Set the magic bits to get a Caterina-based device
    // to reboot into the bootloader and stay there, rather
    // than run move onward
    //
    // These values are the same as those defined in
    // Caterina.c:
    // https://github.com/arduino/ArduinoCore-avr/blob/5755ddea49fa69d6c505c772ebee5af5078e2ebf/bootloaders/caterina/Caterina.c#L130-L133

    uint16_t bootKey = 0x7777;
    uint16_t *const bootKeyPtr = reinterpret_cast<uint16_t *>(0x0800);

    // Stash the magic key
    *bootKeyPtr = bootKey;

    // Set a watchdog timer
    wdt_enable(WDTO_120MS);

    while (true) {} // This infinite loop ensures nothing else
    // happens before the watchdog reboots us
  }
};

}
}
}
}

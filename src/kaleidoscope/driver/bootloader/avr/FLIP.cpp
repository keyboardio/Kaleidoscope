/* -*- mode: c++ -*-
 * kaleidoscope::driver::bootloader::avr::FLIP -- Driver for the Atmel FLIP bootloader
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

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
#include "kaleidoscope/Runtime.h"

#ifdef KALEIDOSCOPE_BOOTLOADER_FLIP_WORKAROUND
#include "kaleidoscope/driver/bootloader/avr/FLIP.h"

namespace kaleidoscope {
namespace driver {
namespace bootloader {
namespace avr {

// The FLIP bootloader does not provide a way to boot into bootloader mode from
// within the firmware: it will always start the user firmware if available, and
// if the reset wasn't triggered by the physical reset button. To enter the
// bootloader mode, we need to jump there from the user firmware ourselves.
//
// Since we do not want to unconditionally jump into bootloader, we set a static
// memory address to a magic value, to signal our intention, then reboot via the
// watchdog timer. We need to reboot to clear registers and have a clean slate
// for the bootloader.
//
// For this to work, we need to run our signal checking code before `main()`, so
// we put the checker function into a special section that does that.
//
// Since this code cannot be optimized out on keyboards that use another
// bootloader, we need to guard this implementation with an ifdef (see above).
//
// For more information, see:
// http://www.fourwalledcubicle.com/files/LUFA/Doc/120730/html/_page__software_bootloader_start.html

#define BOOTLOADER_RESET_KEY 0xB007B007
static uint32_t reset_key __attribute__((section(".noinit")));

/*
 * This function runs before main(), and jumps to the bootloader after a reset
 * initiated by .resetDevice().
 */
static void _bootloader_jump_after_watchdog_reset()
  __attribute__((used, naked, section(".init3")));
static void _bootloader_jump_after_watchdog_reset() {
  if ((MCUSR & (1 << WDRF)) && reset_key == BOOTLOADER_RESET_KEY) {
    reset_key = 0;

    ((void (*)(void))(((FLASHEND + 1L) - 4096) >> 1))();
  }
}

void FLIP::rebootBootloader() {
  reset_key = BOOTLOADER_RESET_KEY;
  wdt_enable(WDTO_250MS);
  while (true) {}  // This infinite loop ensures nothing else
  // happens before the watchdog reboots us
}

}  // namespace avr
}  // namespace bootloader
}  // namespace driver
}  // namespace kaleidoscope

#endif
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

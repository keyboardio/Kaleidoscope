/* -*- mode: c++ -*-
 * Kaleidoscope-FirmwareDump -- Firmware dumper for Kaleidoscope
 * Copyright (C) 2019  Keyboard.io, Inc.
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

#ifdef __AVR__
#ifndef KALEIDOSCOPE_VIRTUAL_BUILD

#include "kaleidoscope/plugin/FirmwareDump.h"

#include <Arduino.h>
#include <Kaleidoscope-FocusSerial.h>
#include <avr/boot.h>

namespace kaleidoscope {
namespace plugin {

EventHandlerResult FirmwareDump::onSetup() {
  enum {
    BOOT_SIZE_4096 = 0b000,
    BOOT_SIZE_2048 = 0b010,
    BOOT_SIZE_1024 = 0b100,
    BOOT_SIZE_512  = 0b110
  };

  uint8_t high_fuse = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);

  if (high_fuse & BOOT_SIZE_512) {
    bootloader_size_ = 512;
  } else if (high_fuse & BOOT_SIZE_1024) {
    bootloader_size_ = 1024;
  } else if (high_fuse & BOOT_SIZE_2048) {
    bootloader_size_ = 2048;
  } else {
    bootloader_size_ = 4096;
  }

  return EventHandlerResult::OK;
}

EventHandlerResult FirmwareDump::onFocusEvent(const char *input) {
  const char *cmd = PSTR("firmware.dump");

  if (::Focus.inputMatchesHelp(input))
    return ::Focus.printHelp(cmd);

  if (!::Focus.inputMatchesCommand(input, cmd))
    return EventHandlerResult::OK;

  uint16_t flash_size = (FLASHEND + 1L);

  for (uint32_t i = 0; i < flash_size - bootloader_size_; i++) {
    uint8_t b = pgm_read_byte(i);
    Focus.send(b);
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::FirmwareDump FirmwareDump;

#endif
#endif

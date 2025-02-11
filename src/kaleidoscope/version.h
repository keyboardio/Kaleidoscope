/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2025 Keyboard.io, inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * Additional Permissions:
 * As an additional permission under Section 7 of the GNU General Public
 * License Version 3, you may link this software against a Vendor-provided
 * Hardware Specific Software Module under the terms of the MCU Vendor
 * Firmware Library Additional Permission Version 1.0.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

// -*- mode: c++ -*-

// clang-format off

#pragma once

// We use `include-what-you-use`, which uses `clang` to help manage header
// includes.  If we don't guard this file with an #ifdef, it will cause that to
// fail on any file that includes this header.
#ifndef __clang__

#define KALEIDOSCOPE_AVR_GCC_MINIMAL_VERSION 5
#define KALEIDOSCOPE_AVR_GCC_MINIMAL_MINOR 4

#if   (__GNUC__  < KALEIDOSCOPE_AVR_GCC_MINIMAL_VERSION) \
  ||  (    (__GNUC__ == KALEIDOSCOPE_AVR_GCC_MINIMAL_VERSION) \
        && (__GNUC_MINOR__ < KALEIDOSCOPE_AVR_GCC_MINIMAL_MINOR) \
      )
#error Kaleidoscope requires Arduino version >= 1.8.6 to build. Please upgrade.
#endif

#endif // #ifndef __clang__

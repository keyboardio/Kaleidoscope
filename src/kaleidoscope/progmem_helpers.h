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

#pragma once

#include <Arduino.h>  // for memcpy_P

// Load any intrinsic data type or trivial class stored in PROGMEM into an
// object of that type in memory.
template<typename _Type>
void loadFromProgmem(_Type const &pgm_object, _Type &object) {
  memcpy_P(&object, &pgm_object, sizeof(object));
}

// Copy an object from PROGMEM to RAM. This works as long as the type has a
// suitable constructor that does not require arguments (i.e. "trivial classes")
// or if `_Type` is an intrinsic data type.
template<typename _Type>
_Type cloneFromProgmem(_Type const &pgm_object) {
  _Type object;
  memcpy_P(&object, &pgm_object, sizeof(object));
  return object;
}

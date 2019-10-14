/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Model01 -- Keyboard.io Model01 hardware support for Kaleidoscope
 * Copyright (C) 2017-2018  Keyboard.io, Inc
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

#ifdef ARDUINO_AVR_MODEL01
#ifndef KALEIDOSCOPE_VIRTUAL_BUILD

#include <Kaleidoscope.h>
#include <KeyboardioHID.h>
#include <avr/wdt.h>

namespace kaleidoscope {
namespace hardware {
namespace keyboardio {

void Model01::setup() {
  Model01KeyScanner::setup();
  Model01Leds::setup();

  TWBR = 12; // This is 400mhz, which is the fastest we can drive the ATTiny
}

void Model01::enableHardwareTestMode() {
  // Toggle the programming LEDS on
  PORTD |= (1 << 5);
  PORTB |= (1 << 0);

  // Disable the debouncer on the ATTinys
  KeyboardHardware.setKeyscanInterval(2);
}

}
}
}

HARDWARE_IMPLEMENTATION KeyboardHardware;

#endif
#endif

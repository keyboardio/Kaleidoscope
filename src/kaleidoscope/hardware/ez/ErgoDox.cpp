/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-EZ-ErgoDox -- ErgoDox hardware support for Kaleidoscope
 * Copyright (C) 2018, 2019  Keyboard.io, Inc
 *
 * Based on QMK (commit 8cdb4a915)
 *  (C) Jack Humbert, Erez Zukerman, Oleg Kostyuk
 * Original QMK sources:
 *  - keyboards/ergodox_ez/ergodox_ez.c
 *  - keyboards/ergodox_ez/ergodox_ez.h
 *  - keyboards/ergodox_ez/matrix.c
 *  - tmk_core/common/avr/bootloader.c
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef ARDUINO_AVR_ERGODOX

#include <Kaleidoscope.h>

#include "kaleidoscope/hardware/ez/ErgoDox/ErgoDoxKeyScanner.h"
#include "kaleidoscope/hardware/ez/ErgoDox/ErgoDoxKeyScannerDescription.h"

namespace kaleidoscope {
namespace hardware {
namespace ez {

ATMEGA_KEYSCANNER_DATA(kaleidoscope::hardware::ez::ErgoDoxDeviceDescription::KeyScanner::ATMegaKeyScanner);
MCP23018_KEYSCANNER_DATA(kaleidoscope::hardware::ez::ErgoDoxDeviceDescription::KeyScanner::ExpanderKeyScanner);

// ErgoDox-specific stuff
void ErgoDox::setStatusLED(uint8_t led, bool state) {
  if (state) {
    DDRB |= (1 << (led + 4));
    PORTB |= (1 << (led + 4));
  } else {
    DDRB &= ~(1 << (led + 4));
    PORTB &= ~(1 << (led + 4));
  }
}

void ErgoDox::setStatusLEDBrightness(uint8_t led, uint8_t brightness) {
  (led == 1) ? (OCR1A = brightness) :
  (led == 2) ? (OCR1B = brightness) :
  (OCR1C = brightness);
}

}
}
}

HARDWARE_IMPLEMENTATION KeyboardHardware;
kaleidoscope::hardware::ez::ErgoDox &ErgoDox = KeyboardHardware;

#endif

/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-KBDFans-KBD4x -- KBD4x hardware support for Kaleidoscope
 * Copyright (C) 2019  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef ARDUINO_AVR_KBD4X

#include <Kaleidoscope.h>
#include <avr/wdt.h>
#include <avr/boot.h>


namespace kaleidoscope {
namespace hardware {
namespace kbdfans {

ATMEGA_KEYBOARD_DATA(KBD4x);
constexpr int8_t KBD4x::led_count;

}
}
}

HARDWARE_IMPLEMENTATION KeyboardHardware;
kaleidoscope::hardware::kbdfans::KBD4x &KBD4x = KeyboardHardware;

#endif

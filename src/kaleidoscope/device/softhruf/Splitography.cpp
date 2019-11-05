/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-SOFTHRUF-Splitography -- Splitography hardware support for Kaleidoscope
 * Copyright (C) 2018, 2019  Keyboard.io, Inc
 *
 * Based on QMK (commit e9a67f8fd) and sdothum's fork (commit 8616b44)
 *  (C) Jack Humbert, Jun Wako, Steven Hum, and others
 * Original QMK sources:
 *  - keyboards/splitography/config.h
 *  - keyboards/splitography/splitography.h
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

#ifdef ARDUINO_AVR_SPLITOGRAPHY

#include <Kaleidoscope.h>
#include <avr/wdt.h>

namespace kaleidoscope {
namespace device {
namespace softhruf {

ATMEGA_KEYBOARD_DATA(Splitography);
constexpr int8_t Splitography::led_count;

}
}
}

kaleidoscope::device::softhruf::Splitography &Splitography = kaleidoscope_internal::device;

#endif

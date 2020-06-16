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

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
#ifdef ARDUINO_AVR_KBD4X

#include "kaleidoscope/key_events.h"
#include "kaleidoscope/driver/keyscanner/ATmega.h"
#include "kaleidoscope/driver/keyscanner/Base_Impl.h"
#include "kaleidoscope/Runtime.h"

namespace kaleidoscope {
namespace device {
namespace kbdfans {

ATMEGA_KEYSCANNER_BOILERPLATE

}
}
}

#endif
#endif // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

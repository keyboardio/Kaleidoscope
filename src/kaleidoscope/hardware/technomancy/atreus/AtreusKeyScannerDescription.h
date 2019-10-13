/* -*- mode: c++ -*-
 * AtreusKeyScannerDescription -- Atreus key scanner description
 * Copyright (C) 2018, 2019  Keyboard.io, Inc
 *
 * Based on QMK (commit e9a67f8fd)
 *  (C) Jack Humbert, Jun Wako, Phil Hagelberg, and others
 * Original QMK sources:
 *  - keyboards/atreus/atreus.h
 *  - keyboards/atreus/config.h
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

#pragma once

#ifdef ARDUINO_AVR_ATREUS

#include "kaleidoscope/driver/keyscanner/ATMegaKeyScannerDescription.h"

namespace kaleidoscope {
namespace hardware {
namespace technomancy {

struct AtreusKeyScannerDescription : public kaleidoscope::driver::keyscanner::ATMegaKeyScannerDescription {
#ifdef KALEIDOSCOPE_HARDWARE_ATREUS_PINOUT_ASTAR
  ATMEGA_KEYSCANNER_DESCRIPTION(
    ROW_PIN_LIST({PIN_D0, PIN_D1, PIN_D3, PIN_D2}),
    COL_PIN_LIST({PIN_D7, PIN_C6, PIN_B5, PIN_B4, PIN_E6, PIN_D4, PIN_B6, PIN_F6, PIN_F7, PIN_D6, PIN_B7})
  );
#endif

#ifdef KALEIDOSCOPE_HARDWARE_ATREUS_PINOUT_ASTAR_DOWN
  ATMEGA_KEYSCANNER_DESCRIPTION(
    ROW_PIN_LIST({PIN_D0, PIN_D1, PIN_D3, PIN_D2}),
    COL_PIN_LIST({PIN_B7, PIN_D6, PIN_F7, PIN_F6, PIN_B6, PIN_D4, PIN_E6, PIN_B4, PIN_B5, PIN_C6, PIN_D7})
  );
#endif

#ifdef KALEIDOSCOPE_HARDWARE_ATREUS_PINOUT_LEGACY_TEENSY2
  ATMEGA_KEYSCANNER_DESCRIPTION(
    ROW_PIN_LIST({PIN_D0, PIN_D1, PIN_D2, PIN_D3}),
    COL_PIN_LIST({PIN_F6, PIN_F5, PIN_F4, PIN_B7, PIN_B6, PIN_B5, PIN_B4, PIN_B3, PIN_B2, PIN_B1, PIN_B0})
  );
#endif
};

}
}
}

typedef kaleidoscope::hardware::technomancy::AtreusKeyScannerDescription::KeyAddr KeyAddr;

#endif

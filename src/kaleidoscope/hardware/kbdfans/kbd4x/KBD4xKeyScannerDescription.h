/* -*- mode: c++ -*-
 * KBD4xKeyScannerDescription -- KBD4x scanner description
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

#pragma once

#ifdef ARDUINO_AVR_KBD4X

#include "kaleidoscope/driver/keyscanner/ATMegaKeyScannerDescription.h"

namespace kaleidoscope {
namespace hardware {
namespace kbdfans {

struct KBD4xKeyScannerDescription : public kaleidoscope::driver::keyscanner::ATMegaKeyScannerDescription {
  ATMEGA_KEYSCANNER_DESCRIPTION(
    ROW_PIN_LIST({ PIN_D0, PIN_D1, PIN_D2, PIN_D3 }),
    COL_PIN_LIST({ PIN_F0, PIN_F1, PIN_F4, PIN_F5, PIN_F6, PIN_F7, PIN_B3, PIN_B1, PIN_B0, PIN_D5, PIN_B7, PIN_C7 })
  );
};

}
}
}

typedef kaleidoscope::hardware::kbdfans::KBD4xKeyScannerDescription::KeyAddr KeyAddr;

#endif

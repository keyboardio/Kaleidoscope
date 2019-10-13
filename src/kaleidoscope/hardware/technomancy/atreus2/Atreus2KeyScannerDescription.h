/* -*- mode: c++ -*-
 * Atreus2KeyScannerDescription -- Atreus2 keyscanner description
 * Copyright (C) 2019  Keyboard.io, Inc
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

#ifdef ARDUINO_AVR_ATREUS2

#include "kaleidoscope/driver/keyscanner/ATMegaKeyScannerDescription.h"

namespace kaleidoscope {
namespace hardware {
namespace technomancy {

struct Atreus2KeyScannerDescription : public kaleidoscope::driver::keyscanner::ATMegaKeyScannerDescription {
  ATMEGA_KEYSCANNER_DESCRIPTION(
    ROW_PIN_LIST({PIN_F6, PIN_F5, PIN_F4, PIN_F1}),
    COL_PIN_LIST({PIN_F7, PIN_E2, PIN_C7, PIN_C6, PIN_B6, PIN_B5, PIN_D7, PIN_D6, PIN_D4, PIN_D5, PIN_D3, PIN_D2})
  );
};

}
}
}

typedef kaleidoscope::hardware::technomancy::Atreus2KeyScannerDescription::KeyAddr KeyAddr;

#endif

/* -*- mode: c++ -*-
 * SplitographyKeyScannerDescription -- Splitography keyscanner description
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

#pragma once

#ifdef ARDUINO_AVR_SPLITOGRAPHY

#include "kaleidoscope/driver/keyscanner/ATMegaKeyScannerDescription.h"

namespace kaleidoscope {
namespace hardware {
namespace softhruf {

struct SplitographyKeyScannerDescription : public kaleidoscope::driver::keyscanner::ATMegaKeyScannerDescription {
  ATMEGA_KEYSCANNER_DESCRIPTION(
    ROW_PIN_LIST({ PIN_D0, PIN_D1, PIN_D2, PIN_D3 }),
    COL_PIN_LIST({ PIN_F0, PIN_F1, PIN_F4, PIN_F5, PIN_F6, PIN_F7, PIN_C7, PIN_C6, PIN_B6, PIN_B5, PIN_B4, PIN_D7 })
  );
};

}
}
}

typedef kaleidoscope::hardware::softhruf::SplitographyKeyScannerDescription::KeyAddr KeyAddr;

#endif

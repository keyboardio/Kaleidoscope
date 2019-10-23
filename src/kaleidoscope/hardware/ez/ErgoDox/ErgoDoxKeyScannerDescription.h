/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-EZ-ErgoDox -- ErgoDox hardware support for Kaleidoscope
 * Copyright (C) 2018-2019  Keyboard.io, Inc
 *
 * Based on QMK (commit 8cdb4a915)
 *  (C) Jack Humbert, Erez Zukerman, Oleg Kostyuk
 * Original QMK sources:
 *  - keyboards/ergodox_ez/ergodox_ez.c
 *  - keyboards/ergodox_ez/ergodox_ez.h
 *  - keyboards/ergodox_ez/matrix.c
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

#ifdef ARDUINO_AVR_ERGODOX

#include <Arduino.h>
#include "kaleidoscope/MatrixAddr.h"

typedef kaleidoscope::MatrixAddr<14, 6> KeyAddr;

#include "kaleidoscope/driver/BaseKeyScannerDescription.h"
#include "kaleidoscope/driver/keyscanner/ATMegaKeyScannerDescription.h"
#include "kaleidoscope/hardware/ez/ErgoDox/MCP23018KeyScannerDescription.h"

namespace kaleidoscope {
namespace hardware {
namespace ez {

struct ErgoDoxATMegaKeyScannerDescription : kaleidoscope::driver::keyscanner::ATMegaKeyScannerDescription {
  ATMEGA_KEYSCANNER_DESCRIPTION_WITH_OFFSET(7, 0,
      ROW_PIN_LIST({PIN_B0, PIN_B1, PIN_B2, PIN_B3, PIN_D2, PIN_D3, PIN_C6}),
      COL_PIN_LIST({PIN_F0, PIN_F1, PIN_F4, PIN_F5, PIN_F6, PIN_F7})
                                           );
};

struct ErgoDoxMCP23018KeyScannerDescription : kaleidoscope::hardware::ez::ergodox::MCP23018KeyScannerDescription {
  MCP23018_KEYSCANNER_DESCRIPTION(7, 6);
};

struct ErgoDoxKeyScannerDescription : kaleidoscope::driver::BaseKeyScannerDescription {
  KEYSCANNER_DESCRIPTION(14, 6);
  typedef ErgoDoxATMegaKeyScannerDescription ATMegaKeyScannerDescription;
  typedef ErgoDoxMCP23018KeyScannerDescription ExpanderKeyScannerDescription;
};

}
}
}

#endif

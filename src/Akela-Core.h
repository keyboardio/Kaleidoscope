/* -*- mode: c++ -*-
 * Akela -- Animated Keyboardio Extension Library for Anything
 * Copyright (C) 2016, 2017  Gergely Nagy
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

#ifdef ARDUINO_AVR_MODEL01
#  define AKELA_KEYBOARDIO_MODEL01
#endif

#include <KeyboardioFirmware.h>

namespace Akela {
  namespace Ranges {
    enum {
      AKELA_FIRST = 0xc000,
      OS_FIRST,
      OSM_FIRST   = OS_FIRST,
      OSM_LAST    = OSM_FIRST + 7,
      OSL_FIRST,
      OSL_LAST    = OSL_FIRST + 23,
      OS_LAST     = OSL_LAST,
      DU_FIRST,
      DUM_FIRST   = DU_FIRST,
      DUM_LAST    = DUM_FIRST + (8 << 8),
      DUL_FIRST,
      DUL_LAST    = DUL_FIRST + (24 << 8),
      DU_LAST     = DUL_LAST,
      TD_FIRST,
      TD_LAST     = TD_FIRST + 31,
      LEAD_FIRST,
      LEAD_LAST   = LEAD_FIRST + 7,
      CYCLE,

      AKELA_SAFE_START,
    };
  };
};

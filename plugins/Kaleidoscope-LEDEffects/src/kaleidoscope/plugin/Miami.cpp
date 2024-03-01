/* -*- mode: c++ -*-
 * Kaleidoscope-LEDEffects -- An assorted collection of LED effects for Kaleidoscope
 * Copyright (C) 2016, 2017  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "kaleidoscope/plugin/Miami.h"

#include "kaleidoscope/device/device.h"    // for CRGB
#include "kaleidoscope/plugin/TriColor.h"  // for TriColor

kaleidoscope::plugin::TriColor MiamiEffect("Miami",
                                           CRGB(0x4e, 0xd6, 0xd6),  /* Cyan */
                                           CRGB(0xaf, 0x67, 0xfa)); /* Magenta */

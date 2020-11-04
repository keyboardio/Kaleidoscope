// -*- mode: c++ -*-
/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2020  Keyboard.io, Inc.
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

#pragma once

// Default key values to be used in sketches are defined from the
// `keymap-defaults.h` file, provided by Kaleidoscope.
#include "testing/keymap-defaults.h"

// For keys of interest, the values can be overridden by defining them in the
// `keymap-overrides.h` file, in this directory.
#include "keymap-overrides.h"

// Finally, we include this file so we can refer to key values in the keymap
// more briefly (`key_0AA` vs `Keymap::key_0AA`, both in the
// `kaleidoscope::testing::keymap` namespace).
#include "testing/keymap-aliases.inc"

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

#include <kaleidoscope/key_defs.h>
#include <kaleidoscope/KeyAddr.h>
#include "testing/keymap-defaults.h"

#include <Kaleidoscope-Qukeys.h>

namespace kaleidoscope {
namespace testing {
namespace keymap {

class Keymap : public KeymapDefaults {
 public:
  // Layer 0
  static constexpr Key key_0AA = Key_A;
  static constexpr Key key_0AB = Key_F;
  static constexpr Key key_0AC = Key_X;
};

constexpr KeyAddr key_addr_A = addr_AA;
constexpr KeyAddr key_addr_F = addr_AB;
constexpr KeyAddr key_addr_X = addr_AC;

}  // namespace keymap
}  // namespace testing
}  // namespace kaleidoscope

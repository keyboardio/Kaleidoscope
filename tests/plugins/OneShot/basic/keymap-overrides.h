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

#include <Kaleidoscope-OneShot.h>

namespace kaleidoscope {
namespace testing {
namespace keymap {

class Keymap : public KeymapDefaults {
 public:
  // Layer 0
  static constexpr Key key_0AA = Key_S;
  static constexpr Key key_0AB = Key_D;

  static constexpr Key key_0BA = OSM(LeftShift);
  static constexpr Key key_0BB = OSM(LeftGui);
  static constexpr Key key_0BC = OSM(LeftControl);

  static constexpr Key key_0CA = OSL(1);
};

}  // namespace keymap
}  // namespace testing
}  // namespace kaleidoscope

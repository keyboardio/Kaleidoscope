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

#include <cstdint>

namespace kaleidoscope {
namespace testing {

constexpr uint16_t QUKEYS_HOLD_TIMEOUT           = 200;
constexpr uint8_t QUKEYS_OVERLAP_THRESHOLD       = 0;
constexpr uint8_t QUKEYS_MINIMUM_HOLD_TIME       = 0;
constexpr uint8_t QUKEYS_MIN_PRIOR_INTERVAL      = 0;
constexpr uint8_t QUKEYS_MAX_TAP_REPEAT_INTERVAL = 20;

}  // namespace testing
}  // namespace kaleidoscope

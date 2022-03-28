/* -*- mode: c++ -*-
 * Kaleidoscope-LED-AlphaSquare -- 4x4 pixel LED alphabet
 * Copyright (C) 2017  Keyboard.io, Inc
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

#include <stdint.h>  // for uint16_t

#include "kaleidoscope/plugin/LED-AlphaSquare.h"  // for SYM4x4

namespace kaleidoscope {
namespace plugin {
namespace alpha_square {
namespace symbols {

// clang-format off

/* λ */
static constexpr uint16_t Lambda = SYM4x4(1, 0, 0, 0,
                                          0, 1, 0, 0,
                                          0, 1, 1, 0,
                                          1, 0, 0, 1);
// clang-format on

}  // namespace symbols
}  // namespace alpha_square
}  // namespace plugin
}  // namespace kaleidoscope

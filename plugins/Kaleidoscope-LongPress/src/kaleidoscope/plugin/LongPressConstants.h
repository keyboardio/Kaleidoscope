/* -*- mode: c++ -*-
 * Kaleidoscope-LongPress -- Provide different key strokes on long press
 * Copyright (C) 2024  Keyboard.io, Inc
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

#include <stdint.h>  // for uint8_t, uint16_t

namespace kaleidoscope {
namespace plugin {
namespace longpress {

// A constant to be used as a placeholder for the layer number in case the
// long-press behaviour should act on all layers.
static constexpr uint8_t ALL_LAYERS = -1;
}  // namespace longpress
}  // namespace plugin
}  // namespace kaleidoscope

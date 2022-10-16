/* Kaleidoscope-MouseKeys - Mouse keys for Kaleidoscope.
 * Copyright (C) 2017-2022  Keyboard.io, Inc.
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

#include <stdint.h>  // for uint16_t, uint8_t

#include "kaleidoscope/plugin/mousekeys/MouseWarpModes.h"  // for MOUSE_WARP_GRID_2X2

// Mouse acceleration

namespace kaleidoscope {
namespace plugin {

// Warping commands

constexpr uint8_t WARP_END   = 1 << 0;
constexpr uint8_t WARP_UP    = 1 << 1;
constexpr uint8_t WARP_DOWN  = 1 << 2;
constexpr uint8_t WARP_LEFT  = 1 << 3;
constexpr uint8_t WARP_RIGHT = 1 << 4;

// apparently, the mac discards 15% of the value space for mouse movement.
// need to test this on other platforms

constexpr uint16_t MAX_WARP_WIDTH  = 32767;
constexpr uint16_t MAX_WARP_HEIGHT = 32767;

constexpr uint8_t WARP_ABS_TOP  = 0;
constexpr uint8_t WARP_ABS_LEFT = 0;

namespace mousekeys {

class MouseWrapper {
 public:
  void warp(uint8_t warp_cmd);
  void endWarping();

 private:
  uint16_t next_width;
  uint16_t next_height;
  uint16_t section_top;
  uint16_t section_left;
  bool is_warping = false;

  void beginWarping();
  void resetWarping();
  void warpJump(uint16_t left, uint16_t top, uint16_t height, uint16_t width);
};

}  // namespace mousekeys

extern mousekeys::MouseWrapper MouseWrapper;

}  // namespace plugin
}  // namespace kaleidoscope

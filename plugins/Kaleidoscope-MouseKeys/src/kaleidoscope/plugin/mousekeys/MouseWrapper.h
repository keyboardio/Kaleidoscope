/* Kaleidoscope-MouseKeys - Mouse keys for Kaleidoscope.
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
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

#include "Arduino.h"
#include "kaleidoscope/plugin/mousekeys/MouseWarpModes.h"

// Warping commands

#define WARP_END 1
#define WARP_UP 2
#define WARP_DOWN 4
#define WARP_LEFT 8
#define WARP_RIGHT 16

// apparently, the mac discards 15% of the value space for mouse movement.
// need to test this on other platforms

#define MAX_WARP_WIDTH 32767
#define MAX_WARP_HEIGHT 32767

#define WARP_ABS_TOP 0
#define WARP_ABS_LEFT 0

// Mouse acceleration

namespace kaleidoscope {
namespace plugin {
namespace mousekeys {

class MouseWrapper {
 public:
  static void move(int8_t x, int8_t y);
  static void warp(uint8_t warp_cmd);

  static uint8_t accel_step;
  static uint8_t speed_limit;
  static constexpr uint8_t subpixels_per_pixel = 16;
  static uint8_t warp_grid_size;

 private:
  static uint16_t next_width;
  static uint16_t next_height;
  static uint16_t section_top;
  static uint16_t section_left;
  static bool is_warping;

  static uint8_t acceleration(uint8_t cycles);

  static void beginWarping();
  static void endWarping();
  static void resetWarping();
  static void warpJump(uint16_t left, uint16_t top, uint16_t height, uint16_t width);
};

extern MouseWrapper wrapper;

} // namespace mousekeys
} // namespace plugin
} // namespace kaleidoscope

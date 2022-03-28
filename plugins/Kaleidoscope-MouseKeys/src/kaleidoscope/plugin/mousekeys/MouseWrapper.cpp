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

#include "kaleidoscope/plugin/mousekeys/MouseWrapper.h"

#include <stdint.h>  // for uint16_t

#include "kaleidoscope/Runtime.h"                              // for Runtime
#include "kaleidoscope/device/device.h"                        // for Base<>...
#include "kaleidoscope/driver/hid/keyboardio/AbsoluteMouse.h"  // for Absolu...
#include "kaleidoscope/driver/hid/keyboardio/Mouse.h"          // for Mouse
#include "kaleidoscope/plugin/mousekeys/MouseWarpModes.h"      // for MOUSE_...

namespace kaleidoscope {
namespace plugin {
namespace mousekeys {

uint8_t MouseWrapper::warp_grid_size = MOUSE_WARP_GRID_2X2;
uint16_t MouseWrapper::next_width;
uint16_t MouseWrapper::next_height;
uint16_t MouseWrapper::section_top;
uint16_t MouseWrapper::section_left;
bool MouseWrapper::is_warping;

uint8_t MouseWrapper::accel_step;
uint8_t MouseWrapper::speed_limit = 127;

void MouseWrapper::warpJump(uint16_t left, uint16_t top, uint16_t height, uint16_t width) {
  uint16_t x_center = left + width / 2;
  uint16_t y_center = top + height / 2;
  Runtime.hid().absoluteMouse().moveTo(x_center, y_center, 0);
}

void MouseWrapper::beginWarping() {
  section_left = WARP_ABS_LEFT;
  section_top  = WARP_ABS_TOP;
  next_width   = MAX_WARP_WIDTH;
  next_height  = MAX_WARP_HEIGHT;
  is_warping   = true;
}

void MouseWrapper::endWarping() {
  is_warping = false;
}

void MouseWrapper::resetWarping() {
  if (is_warping == true) {
    beginWarping();
  }
}

void MouseWrapper::warp(uint8_t warp_cmd) {
  if (is_warping == false) {
    beginWarping();
  }

  if (warp_cmd & WARP_END) {
    endWarping();
    return;
  }

  next_width /= warp_grid_size;
  next_height /= warp_grid_size;

  // WARP_UP + WARP_DOWN means "zoom in" to center sector
  if (warp_cmd & WARP_UP && warp_cmd & WARP_DOWN) {
    section_left += next_width;
    section_top += next_height;

    warpJump(section_left, section_top, next_height, next_width);

    return;
  }

  if (warp_cmd & WARP_DOWN) {
    section_top += next_height * (warp_grid_size - 1);
  } else if (!(warp_cmd & WARP_UP)) {
    section_top += next_height;
  }

  if (warp_cmd & WARP_RIGHT) {
    section_left += next_width * (warp_grid_size - 1);
  } else if (!(warp_cmd & WARP_LEFT)) {
    section_left += next_width;
  }

  warpJump(section_left, section_top, next_height, next_width);
}

// To approximate a sine wave, this uses two parabolas. Acceleration begins
// slowly, grows rapidly in the middle, and slows again near the top.
uint8_t MouseWrapper::acceleration(uint8_t cycles) {
  if (cycles < 128) {
    uint16_t c2 = cycles * cycles;
    return 1 + (c2 >> 7);
  } else {
    uint16_t remaining_cycles = 256 - cycles;
    uint16_t c2               = remaining_cycles * remaining_cycles;
    return 255 - (c2 >> 7);
  }
}

void MouseWrapper::move(int8_t x, int8_t y) {
  int16_t moveX               = 0;
  int16_t moveY               = 0;
  static int8_t remainderX    = 0;
  static int8_t remainderY    = 0;
  int16_t effectiveSpeedLimit = speed_limit;

  if (x != 0) {
    moveX = remainderX + (x * acceleration(accel_step));
    if (moveX > effectiveSpeedLimit)
      moveX = effectiveSpeedLimit;
    else if (moveX < -effectiveSpeedLimit)
      moveX = -effectiveSpeedLimit;
  }

  if (y != 0) {
    moveY = remainderY + (y * acceleration(accel_step));
    if (moveY > effectiveSpeedLimit)
      moveY = effectiveSpeedLimit;
    else if (moveY < -effectiveSpeedLimit)
      moveY = -effectiveSpeedLimit;
  }

  endWarping();
  // move by whole pixels, not subpixels
  Runtime.hid().mouse().move(moveX / subpixels_per_pixel, moveY / subpixels_per_pixel);
  // save leftover subpixel movements for later
  remainderX = moveX - moveX / subpixels_per_pixel * subpixels_per_pixel;
  remainderY = moveY - moveY / subpixels_per_pixel * subpixels_per_pixel;
}

MouseWrapper wrapper;

}  // namespace mousekeys
}  // namespace plugin
}  // namespace kaleidoscope

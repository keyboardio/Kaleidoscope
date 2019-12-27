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


// Mouse-related methods
//
//
#include <Kaleidoscope.h>
#include "kaleidoscope/plugin/MouseKeys/MouseWrapper.h"

namespace kaleidoscope {
namespace plugin {

uint8_t MouseWrapper_::warp_grid_size = MOUSE_WARP_GRID_2X2;
uint16_t MouseWrapper_::next_width;
uint16_t MouseWrapper_::next_height;
uint16_t MouseWrapper_::section_top;
uint16_t MouseWrapper_::section_left;
boolean MouseWrapper_::is_warping;

uint8_t MouseWrapper_::accelStep;
uint8_t MouseWrapper_::speedLimit = 127;
uint8_t MouseWrapper_::subpixelsPerPixel = 16;

void MouseWrapper_::warp_jump(uint16_t left, uint16_t top, uint16_t height, uint16_t width) {
  uint16_t x_center = left + width / 2;
  uint16_t y_center = top + height / 2;
  Kaleidoscope.hid().absoluteMouse().moveTo(x_center, y_center, 0);
}

void MouseWrapper_::begin_warping() {
  section_left = WARP_ABS_LEFT;
  section_top = WARP_ABS_TOP;
  next_width = MAX_WARP_WIDTH;
  next_height = MAX_WARP_HEIGHT;
  is_warping = true;
}

void MouseWrapper_::end_warping() {
  is_warping = false;
}

void MouseWrapper_::reset_warping() {
  if (is_warping == true) {
    begin_warping();
  }
}

void MouseWrapper_::warp(uint8_t warp_cmd) {
  if (is_warping == false) {
    begin_warping();
  }

  if (warp_cmd & WARP_END) {
    end_warping();
    return;
  }

  next_width /= warp_grid_size;
  next_height /= warp_grid_size;

  // WARP_UP + WARP_DOWN means "zoom in" to center sector
  if (warp_cmd & WARP_UP && warp_cmd & WARP_DOWN) {
    section_left += next_width;
    section_top += next_height;

    warp_jump(section_left, section_top, next_height, next_width);

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

  warp_jump(section_left, section_top, next_height, next_width);
}

// To approximate a sine wave, this uses two parabolas. Acceleration begins
// slowly, grows rapidly in the middle, and slows again near the top.
uint8_t MouseWrapper_::acceleration(uint8_t cycles) {
  if (cycles < 128) {
    uint16_t c2 = cycles * cycles;
    return 1 + (c2 >> 7);
  } else {
    uint16_t remaining_cycles = 256 - cycles;
    uint16_t c2 = remaining_cycles * remaining_cycles;
    return 255 - (c2 >> 7);
  }
}

// Get the diagonalized version of a value, i.e. value * sqrt(2) / 2. If the
// value ends up being zero, return the original value instead.
static int16_t diagonalize(int16_t value) {
  // 99 / 140 closely approximates sqrt(2) / 2. Since integer division
  // truncates towards zero we do not need to worry about truncation errors.
  int16_t diagonalValue = value * 99 / 140;
  return (diagonalValue == 0 ? value : diagonalValue);
}

void MouseWrapper_::move(int8_t x, int8_t y) {
  int16_t moveX = 0;
  int16_t moveY = 0;
  static int8_t remainderX = 0;
  static int8_t remainderY = 0;
  int16_t effectiveSpeedLimit = speedLimit;

  if (x != 0 && y != 0) {
    // For diagonal movements, we apply a diagonalized speed limit. The
    // effective speed limit is set based on whether we are moving diagonally.
    effectiveSpeedLimit = diagonalize(effectiveSpeedLimit);

    x = diagonalize(x);
    y = diagonalize(y);
  }

  if (x != 0) {
    moveX = remainderX + (x * acceleration(accelStep));
    if (moveX > effectiveSpeedLimit) moveX = effectiveSpeedLimit;
    else if (moveX < -effectiveSpeedLimit) moveX = -effectiveSpeedLimit;
  }

  if (y != 0) {
    moveY = remainderY + (y * acceleration(accelStep));
    if (moveY > effectiveSpeedLimit) moveY = effectiveSpeedLimit;
    else if (moveY < -effectiveSpeedLimit) moveY = -effectiveSpeedLimit;
  }

  end_warping();
  // move by whole pixels, not subpixels
  Kaleidoscope.hid().mouse().move(moveX / subpixelsPerPixel, moveY / subpixelsPerPixel, 0);
  // save leftover subpixel movements for later
  remainderX = moveX - moveX / subpixelsPerPixel * subpixelsPerPixel;
  remainderY = moveY - moveY / subpixelsPerPixel * subpixelsPerPixel;
}
}
}

kaleidoscope::plugin::MouseWrapper_ MouseWrapper;

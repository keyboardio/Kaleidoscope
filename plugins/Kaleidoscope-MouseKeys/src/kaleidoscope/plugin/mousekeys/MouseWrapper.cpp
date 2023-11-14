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

#include "kaleidoscope/plugin/mousekeys/MouseWrapper.h"
#include "kaleidoscope/plugin/MouseKeys.h"

#include <stdint.h>  // for uint16_t, uint8_t

#include "kaleidoscope/Runtime.h"                        // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"                  // for Base<>::HID, VirtualProps:...
#include "kaleidoscope/driver/hid/base/AbsoluteMouse.h"  // for AbsoluteMouse

namespace kaleidoscope {
namespace plugin {
namespace mousekeys {

// uint8_t MouseWrapper::warp_grid_size = MOUSE_WARP_GRID_2X2;
// uint16_t MouseWrapper::next_width;
// uint16_t MouseWrapper::next_height;
// uint16_t MouseWrapper::section_top;
// uint16_t MouseWrapper::section_left;
// bool MouseWrapper::is_warping;

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

  next_width /= ::MouseKeys.getWarpGridSize();
  next_height /= ::MouseKeys.getWarpGridSize();

  // WARP_UP + WARP_DOWN means "zoom in" to center sector
  if (warp_cmd & WARP_UP && warp_cmd & WARP_DOWN) {
    section_left += next_width;
    section_top += next_height;

    warpJump(section_left, section_top, next_height, next_width);

    return;
  }

  if (warp_cmd & WARP_DOWN) {
    section_top += next_height * (::MouseKeys.getWarpGridSize() - 1);
  } else if (!(warp_cmd & WARP_UP)) {
    section_top += next_height;
  }

  if (warp_cmd & WARP_RIGHT) {
    section_left += next_width * (::MouseKeys.getWarpGridSize() - 1);
  } else if (!(warp_cmd & WARP_LEFT)) {
    section_left += next_width;
  }

  warpJump(section_left, section_top, next_height, next_width);
}

}  // namespace mousekeys

mousekeys::MouseWrapper MouseWrapper;

}  // namespace plugin
}  // namespace kaleidoscope

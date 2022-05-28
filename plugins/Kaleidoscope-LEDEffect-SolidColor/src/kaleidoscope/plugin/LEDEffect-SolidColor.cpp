/* Kaleidoscope-LEDEffect-SolidColor - Solid color LED effects for Kaleidoscope.
 * Copyright (C) 2017  Keyboard.io, Inc.
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

#include "kaleidoscope/plugin/LEDEffect-SolidColor.h"

#include "kaleidoscope/KeyAddr.h"            // for KeyAddr
#include "kaleidoscope/device/device.h"      // for CRGB
#include "kaleidoscope/plugin/LEDControl.h"  // for LEDControl

namespace kaleidoscope {
namespace plugin {

void LEDSolidColor::TransientLEDMode::onActivate() {
  ::LEDControl.set_all_leds_to(parent_->r_,
                               parent_->g_,
                               parent_->b_);
}

void LEDSolidColor::TransientLEDMode::refreshAt(KeyAddr key_addr) {
  ::LEDControl.setCrgbAt(key_addr,
                         CRGB(parent_->r_,
                              parent_->g_,
                              parent_->b_));
}

}  // namespace plugin
}  // namespace kaleidoscope

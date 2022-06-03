/* Kaleidoscope-LEDEffect-Chase - A Chase LED effect for Kaleidoscope.
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

#include "kaleidoscope/plugin/LEDEffect-Chase.h"

#include <stdint.h>  // for uint16_t, uint8_t

#include "kaleidoscope/Runtime.h"            // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"      // for CRGB, Device, Base<>::LEDRangeIterator
#include "kaleidoscope/plugin/LEDControl.h"  // for LEDControl

namespace kaleidoscope {
namespace plugin {

void LEDChaseEffect::TransientLEDMode::update() {
  if (!Runtime.has_leds)
    return;

  if (!Runtime.hasTimeExpired(last_update_, parent_->update_delay_)) {
    return;
  }
  last_update_ += parent_->update_delay_;

  // The red LED is at `pos_`; the blue one follows behind. `direction_` is
  // either +1 or -1; `distance_` is the gap between them.
  uint8_t pos2 = pos_ - (direction_ * parent_->distance_);

  // First, we turn off the LEDs that were turned on in the previous update.
  // `pos_` is always in the valid range (0 <= pos_ < LED_COUNT), but after it
  // changes direction, for the first few updates, `pos2` will be out of bounds.
  // Since it's an unsigned integer, even when it would have a value below zero,
  // it underflows and so one test is good for both ends of the range.
  ::LEDControl.setCrgbAt(pos_, CRGB(0, 0, 0));
  if (Runtime.device().LEDs().isValid(pos2))
    ::LEDControl.setCrgbAt(pos2, CRGB(0, 0, 0));

  // Next, we adjust the red light's position. If the direction hasn't changed (the red
  // light isn't out of bounds), we also adjust the blue light's position to match the red
  // one. If the new position puts it out of bounds, we reverse the direction, and bring
  // it back in bounds. When this happens, the blue light "jumps" behind the red one, and
  // will be out of bounds. The simplest way to do this is to assign it a value that is
  // known to be invalid (LED_COUNT).
  pos_ += direction_;
  if (Runtime.device().LEDs().isValid(pos_)) {
    pos2 += direction_;
  } else {
    direction_ = -direction_;
    pos_ += direction_;
    pos2 = Runtime.device().led_count;
  }

  // Last, we turn on the LEDs at their new positions. As before, the blue light (pos2) is
  // only set if it's in the valid LED range.
  ::LEDControl.setCrgbAt(pos_, CRGB(255, 0, 0));
  if (Runtime.device().LEDs().isValid(pos2))
    ::LEDControl.setCrgbAt(pos2, CRGB(0, 0, 255));
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::LEDChaseEffect LEDChaseEffect;

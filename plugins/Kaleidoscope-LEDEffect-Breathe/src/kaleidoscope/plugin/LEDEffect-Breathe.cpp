/* Kaleidoscope-LEDEffect-Breathe - A breathing effect on the LEDs, for Kaleidoscope.
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

#include "kaleidoscope/plugin/LEDEffect-Breathe.h"

#include <stdint.h>  // for uint8_t

#include "kaleidoscope/Runtime.h"                     // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"               // for cRGB
#include "kaleidoscope/plugin/LEDControl.h"           // for LEDControl
#include "kaleidoscope/plugin/LEDControl/LEDUtils.h"  // for breath_compute

namespace kaleidoscope {
namespace plugin {
void LEDBreatheEffect::TransientLEDMode::update() {
  if (!Runtime.has_leds)
    return;

  if (!Runtime.hasTimeExpired(last_update_, update_interval_))
    return;
  last_update_ += update_interval_;

  cRGB color = breath_compute(parent_->hue, parent_->saturation);
  ::LEDControl.set_all_leds_to(color);
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::LEDBreatheEffect LEDBreatheEffect;

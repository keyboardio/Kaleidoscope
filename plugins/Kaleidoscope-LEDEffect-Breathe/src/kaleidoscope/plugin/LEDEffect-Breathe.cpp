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

#include "Kaleidoscope-LEDEffect-Breathe.h"

namespace kaleidoscope {
namespace plugin {
void LEDBreatheEffect::TransientLEDMode::update(void) {
  if (!Runtime.has_leds)
    return;

  if (!Runtime.hasTimeExpired(last_update_, update_interval_))
    return;
  last_update_ += update_interval_;

  cRGB color = breath_compute(parent_->hue, parent_->saturation);
  ::LEDControl.set_all_leds_to(color);
}
}
}

kaleidoscope::plugin::LEDBreatheEffect LEDBreatheEffect;

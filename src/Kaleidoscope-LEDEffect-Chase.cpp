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

#include "Kaleidoscope-LEDEffect-Chase.h"

namespace kaleidoscope {
void LEDChaseEffect::update(void) {
  if (current_chase_counter++ < chase_threshold) {
    return;
  }
  current_chase_counter = 0;
  ::LEDControl.setCrgbAt(pos - (chase_sign * chase_pixels), {0, 0, 0});
  ::LEDControl.setCrgbAt(pos, {0, 0, 0});

  pos += chase_sign;
  if (pos >= (LED_COUNT - 1) || pos <= 0) {
    chase_sign = -chase_sign;
  }
  ::LEDControl.setCrgbAt(pos, {0, 0, 255});
  ::LEDControl.setCrgbAt(pos - (chase_sign * chase_pixels), {255, 0, 0});
}
}

kaleidoscope::LEDChaseEffect LEDChaseEffect;

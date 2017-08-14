/* -*- mode: c++ -*-
 * Kaleidoscope-LEDEffect-BootGreeting -- Small greeting at boot time
 * Copyright (C) 2017  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Kaleidoscope-LEDEffect-BootGreeting.h"
#include "LEDUtils.h"

namespace kaleidoscope {

bool BootGreetingEffect::done_;

void BootGreetingEffect::begin(void) {
  Kaleidoscope.useLoopHook(loopHook);
}

void BootGreetingEffect::loopHook(const bool post_clear) {
  if (!post_clear || done_)
    return;

  if (millis() > 9200) {
    done_ = true;
    return;
  }

  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {
      Key k = Layer.lookupOnActiveLayer(r, c);

      if (k == Key_LEDEffectNext) {
        cRGB color = breath_compute();
        LEDControl.setCrgbAt(r, c, color);
      }
    }
  }
}
}

kaleidoscope::BootGreetingEffect BootGreetingEffect;

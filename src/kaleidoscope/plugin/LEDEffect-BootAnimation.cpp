/* -*- mode: c++ -*-
 * Kaleidoscope-LEDEffect-BootAnimation -- Small greeting at boot time
 * Copyright (C) 2018  Keyboard.io, Inc
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

#include "Kaleidoscope-LEDEffect-BootAnimation.h"

namespace kaleidoscope {
namespace plugin {

bool BootAnimationEffect::done_ = false;
cRGB BootAnimationEffect::color = CRGB(150, 0, 0);
uint16_t BootAnimationEffect::start_time_ = 0;
uint16_t BootAnimationEffect::timeout = 1000;
uint8_t BootAnimationEffect::current_index_ = 0;
const uint8_t BootAnimationEffect::greeting_[11] PROGMEM = {
  Key_K.keyCode,
  Key_E.keyCode,
  Key_Y.keyCode,
  Key_B.keyCode,
  Key_O.keyCode,
  Key_A.keyCode,
  Key_R.keyCode,
  Key_D.keyCode,
  Key_Period.keyCode,
  Key_I.keyCode,
  Key_O.keyCode
};

EventHandlerResult BootAnimationEffect::onSetup() {
  start_time_ = Kaleidoscope.millisAtCycleStart();
  return EventHandlerResult::OK;
}

EventHandlerResult BootAnimationEffect::afterEachCycle() {
  if (!Kaleidoscope.has_leds)
    return EventHandlerResult::OK;

  //If already done or we're not in a ready state, bail
  if (done_) {
    return EventHandlerResult::OK;
  }

  byte row = 255, col = 255;

  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {
      Key k = Layer.lookupOnActiveLayer(r, c);
      Key g;
      g.flags = 0;
      g.keyCode = pgm_read_byte(&greeting_[current_index_]);

      if (k.raw == g.raw) {
        row = r;
        col = c;
        break;
      }
    }
  }

  if ((Kaleidoscope.millisAtCycleStart() - start_time_) > timeout) {
    current_index_++;
    if (current_index_ == sizeof(greeting_))
      done_ = true;

    start_time_ = Kaleidoscope.millisAtCycleStart();
    if (row != 255 && col != 255)
      ::LEDControl.refreshAt(row, col);
    return EventHandlerResult::OK;
  }

  if (row != 255 && col != 255) {
    ::LEDControl.setCrgbAt(row, col, color);
  }

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::BootAnimationEffect BootAnimationEffect;

/* -*- mode: c++ -*-
 * Kaleidoscope-LEDEffect-BootGreeting -- Small greeting at boot time
 * Copyright (C) 2017, 2018  Keyboard.io, Inc
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

#include "Kaleidoscope-LEDEffect-BootGreeting.h"

namespace kaleidoscope {
namespace plugin {

bool BootGreetingEffect::done_ = false;
byte BootGreetingEffect::row_;
byte BootGreetingEffect::col_;
byte BootGreetingEffect::key_row = 255;
byte BootGreetingEffect::key_col = 255;
Key BootGreetingEffect::search_key = Key_LEDEffectNext;
uint8_t BootGreetingEffect::hue = 170;
uint16_t BootGreetingEffect::start_time = 0;
uint16_t BootGreetingEffect::timeout = 9200;

BootGreetingEffect::BootGreetingEffect(byte pos_row, byte pos_col) {
  key_row = pos_row;
  key_col = pos_col;
}

void BootGreetingEffect::findLed(void) {
  if (key_col != 255 && key_row != 255) {
    row_ = key_row;
    col_ = key_col;
    done_ = true;
    return;
  }

  // Find the LED key.
  for (uint8_t r = 0; r < ROWS; r++) {
    for (uint8_t c = 0; c < COLS; c++) {
      Key k = Layer.lookupOnActiveLayer(r, c);

      if (k.raw == search_key.raw) {
        row_ = r;
        col_ = c;
        return;
      }
    }
  }

  // We didn't find the LED key. Let's just pretend we're "done".
  done_ = true;
}

EventHandlerResult BootGreetingEffect::afterEachCycle() {
  //If already done or we're not in a ready state, bail
  if (done_) {
    return EventHandlerResult::OK;
  }

  //If the start time isn't set, set the start time and
  //find the LEDs.
  if (start_time == 0) {
    start_time = millis();
    findLed();
    //the first time, don't do anything.
    return EventHandlerResult::OK;
  }

  //Only run for 'timeout' milliseconds
  if ((millis() - start_time) > timeout) {
    done_ = true;
    ::LEDControl.refreshAt(row_, col_);
    return EventHandlerResult::OK;
  }

  cRGB color = breath_compute(hue);
  ::LEDControl.setCrgbAt(row_, col_, color);

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::BootGreetingEffect BootGreetingEffect;

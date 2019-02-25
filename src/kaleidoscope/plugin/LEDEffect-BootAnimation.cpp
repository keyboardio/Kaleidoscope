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

  KeyAddr key_addr_found;

  for (auto key_addr : KeyAddr::all()) {
    Key k = Layer.lookupOnActiveLayer(key_addr);
    Key g;
    g.flags = 0;
    g.keyCode = pgm_read_word(&greeting_[current_index_]);

    if (k.raw == g.raw) {
      key_addr_found = key_addr;
      break;
    }
  }

  if ((Kaleidoscope.millisAtCycleStart() - start_time_) > timeout) {
    current_index_++;
    if (current_index_ == sizeof(greeting_))
      done_ = true;

    start_time_ = Kaleidoscope.millisAtCycleStart();
    if (key_addr_found.isValid())
      ::LEDControl.refreshAt(KeyAddr(key_addr_found));
    return EventHandlerResult::OK;
  }

  if (key_addr_found.isValid()) {
    ::LEDControl.setCrgbAt(KeyAddr(key_addr_found), color);
  }

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::BootAnimationEffect BootAnimationEffect;

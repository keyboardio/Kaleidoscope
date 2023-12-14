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

#include "kaleidoscope/plugin/LEDEffect-BootAnimation.h"

#include <Arduino.h>  // for PROGMEM, pgm_read_byte
#include <stdint.h>   // for uint16_t, uint8_t

#include "kaleidoscope/KeyAddr.h"               // for KeyAddr, MatrixAddr, MatrixAddr<>::Range
#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"         // for CRGB, cRGB
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/key_defs.h"              // for Key, Key_O, Key_A, Key_B, Key_D, Key_E
#include "kaleidoscope/layers.h"                // for Layer, Layer_
#include "kaleidoscope/plugin/LEDControl.h"     // for LEDControl

namespace kaleidoscope {
namespace plugin {

bool BootAnimationEffect::done_                          = false;
cRGB BootAnimationEffect::color                          = CRGB(150, 0, 0);
uint16_t BootAnimationEffect::start_time_                = 0;
uint16_t BootAnimationEffect::timeout                    = 1000;
uint8_t BootAnimationEffect::current_index_              = 0;
const uint8_t BootAnimationEffect::greeting_[11] PROGMEM = {
  Key_K.getKeyCode(),
  Key_E.getKeyCode(),
  Key_Y.getKeyCode(),
  Key_B.getKeyCode(),
  Key_O.getKeyCode(),
  Key_A.getKeyCode(),
  Key_R.getKeyCode(),
  Key_D.getKeyCode(),
  Key_Period.getKeyCode(),
  Key_I.getKeyCode(),
  Key_O.getKeyCode()};

EventHandlerResult BootAnimationEffect::onSetup() {
  return EventHandlerResult::OK;
}

EventHandlerResult BootAnimationEffect::afterEachCycle() {
  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  //If already done or we're not in a ready state, bail
  if (done_) {
    return EventHandlerResult::OK;
  }

  KeyAddr key_addr_found;

  for (auto key_addr : KeyAddr::all()) {
    Key k = Layer.lookupOnActiveLayer(key_addr);
    Key g(pgm_read_byte(&greeting_[current_index_]),  // key_code
          0);                                         // flags

    if (k == g) {
      key_addr_found = key_addr;
      break;
    }
  }

  if (Runtime.hasTimeExpired(start_time_, timeout)) {
    current_index_++;
    if (current_index_ == sizeof(greeting_))
      done_ = true;

    start_time_ += timeout;
    if (key_addr_found.isValid())
      ::LEDControl.refreshAt(KeyAddr(key_addr_found));
    return EventHandlerResult::OK;
  }

  if (key_addr_found.isValid()) {
    ::LEDControl.setCrgbAt(KeyAddr(key_addr_found), color);
  }

  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::BootAnimationEffect BootAnimationEffect;

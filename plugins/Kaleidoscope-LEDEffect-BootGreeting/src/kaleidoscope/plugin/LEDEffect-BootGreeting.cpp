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

#include "kaleidoscope/plugin/LEDEffect-BootGreeting.h"

#include <stdint.h>  // for uint16_t, uint8_t

#include "kaleidoscope/KeyAddr.h"                     // for KeyAddr, MatrixAddr, MatrixAddr<>::...
#include "kaleidoscope/Runtime.h"                     // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"               // for cRGB
#include "kaleidoscope/event_handler_result.h"        // for EventHandlerResult, EventHandlerRes...
#include "kaleidoscope/key_defs.h"                    // for Key
#include "kaleidoscope/layers.h"                      // for Layer, Layer_
#include "kaleidoscope/plugin/LEDControl.h"           // for LEDControl, Key_LEDEffectNext
#include "kaleidoscope/plugin/LEDControl/LEDUtils.h"  // for breath_compute

namespace kaleidoscope {
namespace plugin {

bool BootGreetingEffect::done_ = false;
KeyAddr BootGreetingEffect::key_addr_;
KeyAddr BootGreetingEffect::user_key_addr;
Key BootGreetingEffect::search_key      = Key_LEDEffectNext;
uint8_t BootGreetingEffect::hue         = 170;
uint16_t BootGreetingEffect::start_time = 0;
uint16_t BootGreetingEffect::timeout    = 9200;

BootGreetingEffect::BootGreetingEffect(KeyAddr key_addr) {
  user_key_addr = key_addr;
}

void BootGreetingEffect::findLed() {
  if (user_key_addr.isValid()) {
    key_addr_ = user_key_addr;
    done_     = true;
    return;
  }

  // Find the LED key.
  for (auto key_addr : KeyAddr::all()) {
    Key k = Layer.lookupOnActiveLayer(key_addr);

    if (k == search_key) {
      key_addr_ = key_addr;
      return;
    }
  }

  // We didn't find the LED key. Let's just pretend we're "done".
  done_ = true;
}

EventHandlerResult BootGreetingEffect::onSetup() {
  findLed();
  return EventHandlerResult::OK;
}

EventHandlerResult BootGreetingEffect::afterEachCycle() {
  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  //If already done or we're not in a ready state, bail
  if (done_) {
    return EventHandlerResult::OK;
  }

  //Only run for 'timeout' milliseconds
  if (Runtime.hasTimeExpired(start_time, timeout)) {
    done_ = true;
    ::LEDControl.refreshAt(key_addr_);
    return EventHandlerResult::OK;
  }

  cRGB color = breath_compute(hue);
  ::LEDControl.setCrgbAt(key_addr_, color);

  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::BootGreetingEffect BootGreetingEffect;

/* -*- mode: c++ -*-
 * Kaleidoscope-Escape-OneShot -- Turn ESC into a key that cancels OneShots, if active.
 * Copyright (C) 2016, 2017, 2018  Keyboard.io, Inc
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

#include <stdint.h>                               // for uint8_t
#include "kaleidoscope/Runtime.h"      // for Kaleidoscope, Kalei...
#include "kaleidoscope/KeyAddr.h"                 // for KeyAddr
#include "kaleidoscope/device/device.h"           // for Device
#include "kaleidoscope/event_handler_result.h"    // for EventHandlerResult
#include "kaleidoscope/key_defs.h"                // for Key, Key_Escape
#include "kaleidoscope/keyswitch_state.h"         // for INJECTED, keyToggledOn
#include "kaleidoscope/plugin/Escape-OneShot.h"   // for EscapeOneShot
#include "kaleidoscope/plugin/OneShot.h"          // for OneShot

namespace kaleidoscope {
namespace plugin {

EventHandlerResult EscapeOneShot::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t keyState) {
  if (mapped_key != Key_Escape ||
      (keyState & INJECTED) ||
      !keyToggledOn(keyState))
    return EventHandlerResult::OK;

  if ((!::OneShot.isActive() || ::OneShot.isPressed()) && !::OneShot.isSticky()) {
    return EventHandlerResult::OK;
  }

  Runtime.device().maskKey(key_addr);

  ::OneShot.cancel(true);
  return EventHandlerResult::EVENT_CONSUMED;
}

}
}

kaleidoscope::plugin::EscapeOneShot EscapeOneShot;

/* -*- mode: c++ -*-
 * Kaleidoscope-TopsyTurvy -- Turn the effect of Shift upside down for certain keys
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

#include "kaleidoscope/plugin/TopsyTurvy.h"

#include <Kaleidoscope-FocusSerial.h>  // for Focus
#include <Kaleidoscope-Ranges.h>       // for TT_FIRST

#include "kaleidoscope/KeyAddr.h"                         // for KeyAddr, MatrixAddr, MatrixAddr...
#include "kaleidoscope/KeyEvent.h"                        // for KeyEvent
#include "kaleidoscope/LiveKeys.h"                        // for LiveKeys, live_keys
#include "kaleidoscope/Runtime.h"                         // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"                   // for Base<>::HID, VirtualProps::HID
#include "kaleidoscope/driver/hid/keyboardio/Keyboard.h"  // for Keyboard
#include "kaleidoscope/event_handler_result.h"            // for EventHandlerResult, EventHandle...
#include "kaleidoscope/key_defs.h"                        // for Key, Key_LeftShift, Key_NoKey
#include "kaleidoscope/keyswitch_state.h"                 // for keyToggledOff

namespace kaleidoscope {
namespace plugin {

EventHandlerResult TopsyTurvy::onNameQuery() {
  return ::Focus.sendName(F("TopsyTurvy"));
}

EventHandlerResult TopsyTurvy::onKeyEvent(KeyEvent &event) {
  if (keyToggledOff(event.state)) {
    if (event.addr == tt_addr_)
      tt_addr_.clear();
    return EventHandlerResult::OK;
  }

  if (event.key.isKeyboardModifier())
    return EventHandlerResult::OK;

  if (isTopsyTurvyKey(event.key)) {
    event.key.setRaw(event.key.getRaw() - ranges::TT_FIRST);
    tt_addr_ = event.addr;
  } else {
    live_keys.activate(tt_addr_, Key_NoKey);
    tt_addr_.clear();
  }

  if (tt_addr_.isValid()) {
    for (KeyAddr key_addr : KeyAddr::all()) {
      if (key_addr == event.addr)
        continue;

      Key active_key = live_keys[key_addr];
      if (active_key == Key_Transparent)
        continue;

      if (active_key.isKeyboardKey() && !active_key.isKeyboardModifier()) {
        live_keys.activate(key_addr, Key_NoKey);
      }
    }
  }
  return EventHandlerResult::OK;
}

EventHandlerResult TopsyTurvy::beforeReportingState(const KeyEvent &event) {

  if (!tt_addr_.isValid()) {
    return EventHandlerResult::OK;
  }
  // If a TopsyTurvy key is being held, no other KeyboardKey should be able to
  // toggle off, because those keys were masked. It's possible for other plugins
  // to change that, but those types of complex plugin interactions can't be
  // guaranteed to be safe, anyway. Therefore, we assume that if `tt_addr` is
  // valid, it is also the last key pressed.
  bool shift_detected = false;
  for (KeyAddr key_addr : KeyAddr::all()) {
    if (live_keys[key_addr].isKeyboardShift()) {
      shift_detected = true;
      break;
    }
  }

  if (shift_detected) {
    Runtime.hid().keyboard().releaseKey(Key_LeftShift);
    Runtime.hid().keyboard().releaseKey(Key_RightShift);
  } else {
    Runtime.hid().keyboard().pressKey(Key_LeftShift);
  }

  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::TopsyTurvy TopsyTurvy;

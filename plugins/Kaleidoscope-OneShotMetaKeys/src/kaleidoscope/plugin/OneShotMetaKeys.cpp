/* -*- mode: c++ -*-
 * Kaleidoscope-OneShot -- One-shot modifiers and layers
 * Copyright (C) 2021  Keyboard.io, Inc.
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

#include "kaleidoscope/plugin/OneShotMetaKeys.h"

#include <Arduino.h>                   // for F, __FlashStringHelper
#include <Kaleidoscope-FocusSerial.h>  // for Focus, FocusSerial
#include <Kaleidoscope-OneShot.h>      // for OneShot

#include "kaleidoscope/KeyAddr.h"               // for MatrixAddr, MatrixAddr<>::Range, KeyAddr
#include "kaleidoscope/KeyAddrMap.h"            // for KeyAddrMap<>::Iterator, KeyAddrMap
#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/KeyMap.h"                // for KeyMap
#include "kaleidoscope/LiveKeys.h"              // for LiveKeys, live_keys
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/key_defs.h"              // for Key, Key_Inactive, Key_Masked
#include "kaleidoscope/keyswitch_state.h"       // for INJECTED, keyToggledOff

namespace kaleidoscope {
namespace plugin {

// ============================================================================
// Public interface

// ----------------------------------------------------------------------------
// Plugin hook functions

EventHandlerResult OneShotMetaKeys::onNameQuery() {
  return ::Focus.sendName(F("OneShotMetaKeys"));
}

EventHandlerResult OneShotMetaKeys::onKeyEvent(KeyEvent &event) {
  // Ignore key release and injected events.
  if (keyToggledOff(event.state) || event.state & INJECTED)
    return EventHandlerResult::OK;

  // Make all held keys sticky if `OneShot_ActiveStickyKey` toggles on.
  if (event.key == OneShot_ActiveStickyKey) {
    // Note: we don't need to explicitly skip the key the active sticky key
    // itself (i.e. `event.addr`), because its entry in `live_keys[]` has not
    // yet been inserted at this point.
    for (KeyAddr addr : KeyAddr::all()) {
      // Get the entry from the keyboard state array.
      Key key = live_keys[addr];
      // Skip idle and masked entries.
      if (key == Key_Inactive || key == Key_Masked) {
        continue;
      }
      // Make everything else sticky.
      ::OneShot.setSticky(addr);
    }
    return EventHandlerResult::OK;
  }

  // If there's an active meta-sticky key, we need to make newly-pressed keys
  // sticky, unless they were already active, in which case we let OneShot
  // release them from the "sticky" state.
  if (isMetaStickyActive() &&
      live_keys[event.addr] != event.key) {
    ::OneShot.setSticky(event.addr);
    return EventHandlerResult::OK;
  }

  // If a previously-inactive meta-sticky key was just pressed, we have OneShot
  // put it in the "pending" state so it will act like a OneShot key.
  if (event.key == OneShot_MetaStickyKey &&
      live_keys[event.addr] != OneShot_MetaStickyKey) {
    ::OneShot.setPending(event.addr);
  }
  return EventHandlerResult::OK;
}

bool OneShotMetaKeys::isMetaStickyActive() {
  for (Key key : live_keys.all()) {
    if (key == OneShot_MetaStickyKey)
      return true;
  }
  return false;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::OneShotMetaKeys OneShotMetaKeys;

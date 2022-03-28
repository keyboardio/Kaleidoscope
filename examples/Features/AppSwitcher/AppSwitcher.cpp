/* -*- mode: c++ -*-
 * AppSwitcher -- A Kaleidoscope Example
 * Copyright (C) 2021  Keyboardio, Inc.
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

#define KALEIDOSCOPE_HOSTOS_GUESSER 1

#include <Kaleidoscope-HostOS.h>

#include "AppSwitcher.h"

namespace kaleidoscope {
namespace plugin {

EventHandlerResult AppSwitcher::onKeyEvent(KeyEvent &event) {
  // Ignore all key releases
  if (keyToggledOff(event.state))
    return EventHandlerResult::OK;

  if (event.key == AppSwitcher_Next || event.key == AppSwitcher_Prev) {
    bool add_shift_flag = false;
    if (event.key == AppSwitcher_Prev) {
      add_shift_flag = true;
    }

    // For good measure:
    event.state |= INJECTED;

    // If AppSwitcher was not already active, hold its modifier first.
    if (!active_addr_.isValid()) {
      if (::HostOS.os() == hostos::MACOS) {
        event.key = Key_LeftGui;
      } else {
        event.key = Key_LeftAlt;
      }
      Runtime.handleKeyEvent(event);
    }

    // Clear the event's key address so we don't clobber the modifier.
    event.addr.clear();
    event.key = Key_Tab;
    if (add_shift_flag)
      event.key.setFlags(SHIFT_HELD);
    // Press tab
    Runtime.handleKeyEvent(event);
    // Change state to release; this will get processed when we return OK below.
    event.state = WAS_PRESSED | INJECTED;
  } else if (active_addr_.isValid()) {
    // If any non-AppSwitcher key is pressed while AppSwitcher is active, that
    // will close AppSwitcher instead of processing that keypress. We mask the
    // address of the key that closed AppSwitcher so that its release doesn't
    // have any effect. Then we turn the event for that key's press into an
    // event for the release of the AppSwitcher's modifier key.
    live_keys.mask(event.addr);
    event.addr  = active_addr_;
    event.state = WAS_PRESSED | INJECTED;
    event.key   = live_keys[event.addr];
    // Turn off AppSwitcher:
    active_addr_.clear();
  }
  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::AppSwitcher AppSwitcher;

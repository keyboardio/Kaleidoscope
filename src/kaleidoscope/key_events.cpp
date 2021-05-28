/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2018  Keyboard.io, Inc.
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

#ifndef NDEPRECATED
#include "kaleidoscope/Runtime.h"
#include "kaleidoscope/LiveKeys.h"
#include "kaleidoscope/hooks.h"
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/layers.h"
#include "kaleidoscope/event_handler_result.h"

// Deprecated. See `Runtime.handleKeyEvent()`
void handleKeyswitchEvent(Key key, KeyAddr key_addr, uint8_t key_state) {
  // Perhaps we should call deprecated plugin event handlers here?
  auto result = kaleidoscope::Hooks::onKeyswitchEvent(key, key_addr, key_state);
  if (result == kaleidoscope::EventHandlerResult::ABORT)
    return;
  if (keyIsPressed(key_state))
    kaleidoscope::Runtime.addToReport(key);
}
#endif

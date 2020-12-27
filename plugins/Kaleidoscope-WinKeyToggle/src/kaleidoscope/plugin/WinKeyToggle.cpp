/* -*- mode: c++ -*-
 * Kaleidoscope-WinKeyToggle -- Toggle the Windows (GUI) key on/off
 * Copyright (C) 2018  Keyboard.io, Inc.
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

#include "kaleidoscope/Runtime.h"
#include <Kaleidoscope-WinKeyToggle.h>

namespace kaleidoscope {
namespace plugin {

bool WinKeyToggle::enabled_;

EventHandlerResult WinKeyToggle::onKeyswitchEvent(Key &key, KeyAddr key_addr, uint8_t key_state) {
  if (!enabled_)
    return EventHandlerResult::OK;

  if (key == Key_LeftGui || key == Key_RightGui)
    return EventHandlerResult::EVENT_CONSUMED;

  return EventHandlerResult::OK;
}
}
}

kaleidoscope::plugin::WinKeyToggle WinKeyToggle;

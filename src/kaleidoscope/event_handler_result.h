/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2022  Keyboard.io, Inc.
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

#pragma once

#include <stdint.h>  // for uint8_t

namespace kaleidoscope {

// This is the set of return values for event handlers. Event handlers for
// plugins are called in sequence by the corresponding hook function, in plugin
// initialization order. The interpretation of these return values can vary
// based on the needs of the hook function, but should be as follows:
//
// - OK: Continue processing the event. The calling hook function should
//       continue calling next event handler in the sequence. If all event
//       handlers return `OK`, finish processing the event.
//
// - EVENT_CONSUMED: Stop processing event handlers. The calling hook function
//       should not call any further handlers, but may continue to take some
//       actions to finish processing the event. This should be used to indicate
//       that the event has been successfully handled.
//
// - ABORT: Ignore the event. The calling hook function should not call any
//       further handlers, and should treat the event as if it didn't
//       happen. This should be used by plugin handlers that need to either
//       suppress an event or queue the event in order to delay it.
//
// - ERROR: Undefined error. The calling hook function should not call any
//       further handlers. There is currently no specification for what should
//       happen if this is returned.

enum class EventHandlerResult : uint8_t {
  OK,
  EVENT_CONSUMED,
  ABORT,
  ERROR,
};

}  // namespace kaleidoscope

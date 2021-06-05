/* Kaleidoscope - Firmware for computer input devices
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

#include <Arduino.h>

#include "kaleidoscope/Runtime.h"
#include "Kaleidoscope-FocusSerial.h"

namespace kaleidoscope {
namespace plugin {

// =============================================================================
// Event Handlers

// -----------------------------------------------------------------------------
EventHandlerResult Pointer::onNameQuery() {
  return ::Focus.sendName(F("Pointer"));
}

// -----------------------------------------------------------------------------
EventHandlerResult Pointer::onSetup(void) {
  kaleidoscope::Runtime.hid().mouse().setup();

  return EventHandlerResult::OK;
}

// -----------------------------------------------------------------------------
EventHandlerResult Pointer::afterEachCycle() {
  Runtime.hid().mouse().sendReport();

  return EventHandlerResult::OK;
}

// -----------------------------------------------------------------------------
EventHandlerResult Pointer::onPointerSensorEvent(PointerEvent &event) {
  if (event.key != POINTER_MOTION && event.key != POINTER_SCROLL)
    return EventHandlerResult::OK;

  if (event.key == POINTER_MOTION) {
    Runtime.hid().mouse().move(event.x, event.y, 0, 0);
  } else if (event.key == POINTER_SCROLL) {
    Runtime.hid().mouse().move(0, 0, event.v, event.h);
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

} // namespace plugin
} // namespace kaleidoscope

kaleidoscope::plugin::Pointer Pointer;
